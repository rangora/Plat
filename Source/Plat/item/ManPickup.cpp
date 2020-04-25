// Fill out your copyright notice in the Description page of Project Settings.

#include "ManPickup.h"

AManPickup::AManPickup() {
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	ItemID = FName("NO ID");
	Super::Name = "Item";
	Super::Action = "pickup";
}

void AManPickup::Interact(APlayerController* Controller) {
	Super::Interact(Controller);

	AAvatarController* IController = Cast<AAvatarController>(Controller);

	if (IController->AddItemToInventory(ItemID)) {
		Destroy();
	}
}