// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Actor.h"
#include "system/AvatarController.h"
#include "AutoPickup.generated.h"

/**
This class is used to implement items which are picked up automatically
when the player is in pickup range to the actors.
*/
UCLASS()
class PLAT_API AAutoPickup : public AActor {
	GENERATED_BODY()

public:
	AAutoPickup();

	virtual void Collect_Implementation(APlayerController* Controller);

	FName GetItemID();

	UFUNCTION(BlueprintNativeEvent)
		void Collect(APlayerController* Controller);

protected:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
};
