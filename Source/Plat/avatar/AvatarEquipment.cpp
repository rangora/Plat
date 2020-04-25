// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarEquipment.h"

AAvatarEquipment::AAvatarEquipment() {
	PrimaryActorTick.bCanEverTick = false;
	
	Name = "None";
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
	RootComponent = StaticMeshComponent;
}

void AAvatarEquipment::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AAvatarEquipment::SetWeaponStaticMesh(const FString& ContentPath, const FString& Name) {
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(
		UStaticMesh::StaticClass(), NULL, *ContentPath));
	
	StaticMeshComponent->SetStaticMesh(StaticMesh);

	this->Name = Name;
}

FString AAvatarEquipment::GetWeaponName() {
	return Name;
}

void AAvatarEquipment::BeginPlay() {
	Super::BeginPlay();	
}

