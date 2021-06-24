// Fill out your copyright notice in the Description page of Project Settings.

#include "AutoPickup.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"


AAutoPickup::AAutoPickup() {
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetCollisionProfileName(TEXT("Item"));
	PickupMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	RootComponent = Cast<USceneComponent>(PickupMesh);

	rotateSpeed = 55.f;
	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	Rotation->RotationRate = FRotator(0.f, rotateSpeed, 0.f);

	ItemID = FName("No_ID");
}

void AAutoPickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, rotateSpeed * DeltaTime, 0.f));
}

void AAutoPickup::Collect_Implementation(APlayerController* Controller) {
	float speed = 0.03;

	AAvatarController* IController = Cast<AAvatarController>(Controller);
	AAvatar* IPlayer = Cast<AAvatar>(IController->GetCharacter());

	FVector PlayerLocation = IPlayer->GetActorLocation();
	FVector Direction = PlayerLocation - GetTransform().GetLocation();
	FVector NewLocation = GetTransform().GetLocation() + (Direction * speed);

	bIsCollecting = true;
	PickupMesh->SetEnableGravity(false);

	PlayerLocation.Z += 55.f;
	SetActorLocation(NewLocation);
}

FName AAutoPickup::GetItemID() {
	return ItemID;
}