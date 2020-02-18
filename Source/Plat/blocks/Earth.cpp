// Fill out your copyright notice in the Description page of Project Settings.


#include "Earth.h"

// Sets default values
AEarth::AEarth() {
 	PrimaryActorTick.bCanEverTick = false;

	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EARTH"));
	RootComponent = Block;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BLOCK(
		TEXT("/Game/resources/blocks/earth.earth"));

	if (SM_BLOCK.Succeeded())
		Block->SetStaticMesh(SM_BLOCK.Object);
}

void AEarth::BeginPlay() {
	Super::BeginPlay();
}

void AEarth::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AEarth::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

