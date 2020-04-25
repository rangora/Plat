// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoPickup.h"

AAutoPickup::AAutoPickup() {
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	ItemID = FName("No ID");
}

void AAutoPickup::Collect_Implementation(APlayerController* Controller) {
	AAvatarController* IController = Cast<AAvatarController>(Controller);
	if (IController->AddItemToInventory(ItemID))
		Destroy();
}

FName AAutoPickup::GetItemID() {
	return FName();
}