// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/GSPickupableInterface.h"
#include "Interaction/GSInteractableActor.h"
#include "GSEquipmentPickupActor.generated.h"

class UGSPickupDefinition;
class UCapsuleComponent;
class UGSInventoryItemDefinition;

UCLASS()
class GASSHOOTER_API AGSEquipmentPickupActor : public AGSInteractableActor, public IGSPickupableInterface
{
	GENERATED_BODY()
	
public:	

	AGSEquipmentPickupActor(const FObjectInitializer& ObjectInitializer);
	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION(BlueprintNativeEvent)
	void AttemptPickUpEquipment(APawn* Pawn);

	/** Searches an item definition type for a matching stat and returns the value, or 0 if not found */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GS|EquipmentPickup")
	static int32 GetDefaultStatFromItemDef(const TSubclassOf<UGSInventoryItemDefinition> EquipmentClass, FGameplayTag StatTag);

	/* Pickupable Interface*/
	virtual TSubclassOf<UGSInventoryItemDefinition> GetItemDefinition_Implementation() const override;
	virtual bool GiveEquipment_Implementation(TSubclassOf<UGSInventoryItemDefinition> EquipmentClass, APawn* ReceivingPawn) override;
	/* End Pickupable Interface*/

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|EquipmentPickup")
	TObjectPtr<UGSPickupDefinition> EquipmentDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GS|EquipmentPickup")
	TObjectPtr<UStaticMeshComponent> EquipmentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "GS|EquipmentPickup")
	bool bUseCollisionVolume = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GS|EquipmentPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;
};