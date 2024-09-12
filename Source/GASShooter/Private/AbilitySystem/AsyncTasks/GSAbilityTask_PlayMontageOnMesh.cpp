// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/GSAbilityTask_PlayMontageOnMesh.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGSAbilityTask_PlayMontageOnMesh* UGSAbilityTask_PlayMontageOnMesh::CreatePlayMontageOnMeshProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	USkeletalMeshComponent* InSkeletalMeshComponent, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, 
	float AnimRootMotionTranslationScale, float StartTimeSeconds, bool bAllowInterruptAfterBlendOut)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
	
	UGSAbilityTask_PlayMontageOnMesh* MyObj = NewAbilityTask<UGSAbilityTask_PlayMontageOnMesh>(OwningAbility, TaskInstanceName);
	MyObj->SkeletalMeshComponent = InSkeletalMeshComponent;
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->bAllowInterruptAfterBlendOut = bAllowInterruptAfterBlendOut;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UGSAbilityTask_PlayMontageOnMesh::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	if (SkeletalMeshComponent == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (UGSAbilitySystemComponent* GSASC = Cast<UGSAbilitySystemComponent>(AbilitySystemComponent.Get()))
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		if (AnimInstance == nullptr)
		{
			return;
		}

		if (GSASC->PlayMontageOnMesh(Ability, Ability->GetCurrentActivationInfo(), SkeletalMeshComponent, MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
		{
			// make sure we aren't pending kill to not fire unwanted delegates
			if (ShouldBroadcastAbilityTaskDelegates() == false)
			{
				return;
			}

			InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &ThisClass::OnGameplayAbilityCancelled);

			BlendingOutDelegate.BindUObject(this, &ThisClass::OnMontageBlendingOut);
			SkeletalMeshComponent->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

			MontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded);
			SkeletalMeshComponent->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
				(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
			}

			bPlayedMontage = true;
		}
	}

	if (!bPlayedMontage)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast();
		}
	}

	SetWaitingOnAvatar();
}

void UGSAbilityTask_PlayMontageOnMesh::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	const bool bPlayingThisMontage = (Montage == MontageToPlay) && Ability && Ability->GetCurrentMontage() == MontageToPlay;
	if (bPlayingThisMontage)
	{
		if (bPlayingThisMontage)
		{
			// Reset AnimRootMotionTranslationScale
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
				(Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}
		}
	}

	if (bPlayingThisMontage && (bInterrupted || !bAllowInterruptAfterBlendOut))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->ClearAnimatingAbility(Ability);
		}
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (bInterrupted)
		{
			OnInterrupted.Broadcast();
			EndTask();
		}
		else
		{
			OnBlendOut.Broadcast();
		}
	}
}

void UGSAbilityTask_PlayMontageOnMesh::OnGameplayAbilityCancelled()
{
	if (StopPlayingMontage() || bAllowInterruptAfterBlendOut)
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast();
		}
	}
	
	EndTask();
}

void UGSAbilityTask_PlayMontageOnMesh::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast();
		}
	}

	EndTask();
}

void UGSAbilityTask_PlayMontageOnMesh::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.Broadcast();
	}
	Super::ExternalCancel();
}

void UGSAbilityTask_PlayMontageOnMesh::OnDestroy(bool AbilityEnded)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptedHandle);
		if (AbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	Super::OnDestroy(AbilityEnded);
}

bool UGSAbilityTask_PlayMontageOnMesh::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	if (SkeletalMeshComponent == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (ASC->GetAnimatingAbility() == Ability
			&& ASC->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			ASC->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

FString UGSAbilityTask_PlayMontageOnMesh::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability)
	{
		if (SkeletalMeshComponent == nullptr)
		{
			return FString::Printf(TEXT("PlayMontageAndWait. SkeletalMesh : NULL. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
		}

		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? ToRawPtr(MontageToPlay) : AnimInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWait. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}
