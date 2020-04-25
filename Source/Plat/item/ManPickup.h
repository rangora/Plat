// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "system/AvatarController.h"
#include "item/Interactable.h"
#include "ManPickup.generated.h"

/**
 * It is used to implement items which can be picked up manually by the player.
 The player has to be in pickup range,
 has to look directly at the item and he has to press a specific button to pick up the item.
 */
UCLASS()
class PLAT_API AManPickup : public AInteractable {
	GENERATED_BODY()

public:
	AManPickup();

	void Interact(APlayerController* Controller) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;
};
