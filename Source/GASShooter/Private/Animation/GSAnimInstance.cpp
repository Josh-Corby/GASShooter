// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GSAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "Character/GSCharacterBase.h"
#include "Character/GSCharacterMovementComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif //WITH_EDitor

UGSAnimInstance::UGSAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* InASC)
{
	check(InASC);

	GameplayTagPropertyMap.Initialize(this, InASC);
}

EDataValidationResult UGSAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}

void UGSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UGSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AGSCharacterBase* Character = Cast<AGSCharacterBase>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UGSCharacterMovementComponent* CharMoveComp = CastChecked<UGSCharacterMovementComponent>(Character->GetCharacterMovement());
	const FGSCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}
