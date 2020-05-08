// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment.h"

AEquipment::AEquipment() {
	PrimaryActorTick.bCanEverTick = false;

	damage = -1.f;
	adDamage = -1.f;
	Match = EMatch::NONE;
	Name = "None";
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	RootComponent = StaticMeshComponent;
}

void AEquipment::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

bool AEquipment::SetWeaponStaticMesh(const FString& ContentPath, const FString& Name) {
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(
		UStaticMesh::StaticClass(), NULL, *ContentPath));

	if (IsValid(StaticMesh)) {
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		FVector CurrentScale = StaticMeshComponent->GetRelativeScale3D();
		StaticMeshComponent->SetRelativeScale3D(CurrentScale * 2.f);
		this->Name = Name;
		return true;
	}
	return false;
}

FString AEquipment::GetWeaponName() {
	return Name;
}

void AEquipment::BeginPlay() {
	Super::BeginPlay();
}