// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSSpawnVolume.generated.h"

class UBoxComponent;
class AGSSpawnPoint;

UCLASS()
class GASSHOOTER_API AGSSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	

	AGSSpawnVolume();

	UPROPERTY(BlueprintReadOnly)
	bool bReached = false;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<AGSSpawnPoint*> SpawnPoints;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
