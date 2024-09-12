// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Teams/GSTeamAgentInterface.h"
#include "GSPlayerController.generated.h"

class AGSPlayerState;
class UInputMappingContext;
class UGSInputConfig;
class UGSAbilitySystemComponent;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSPlayerController : public APlayerController, public IGSTeamAgentInterface
{
	GENERATED_BODY()
	
public:

	AGSPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


	// AController interface
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	// End AController interface

	// APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	// End APlayerController interface

	UFUNCTION(BlueprintCallable, Category = "GS|PlayerController")
	AGSPlayerState* GetGSPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "GS|PlayerController")
	UGSAbilitySystemComponent* GetGSAbilitySystemComponent() const;

	// IGSTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate();
	// End IGSTeamAgentInterface

protected:

	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void Crouch(const FInputActionValue& InputActionValue);
	void Sprint(const FInputActionValue& InputActionValue);

private:

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeamID, int32 NewTeamID);

	void BroadcastOnPlayerStateChanged();

private:

	UPROPERTY(EditDefaultsOnly, Category = "GS|Input")
	TObjectPtr<UInputMappingContext> GSContext;

	UPROPERTY(EditDefaultsOnly, Category = "GS|Input")
	TObjectPtr<UGSInputConfig> InputConfig;

	UPROPERTY()
	FOnGSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

	UPROPERTY()
	bool bIsSprinting = false;
};
