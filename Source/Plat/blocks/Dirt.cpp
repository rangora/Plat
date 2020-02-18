// Fill out your copyright notice in the Description page of Project Settings.


#include "Dirt.h"

// Sets default values
ADirt::ADirt() {
	PrimaryActorTick.bCanEverTick = false;

	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DIRT"));
	RootComponent = Block;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BLOCK(
		TEXT("/Game/resources/blocks/dirt.dirt"));
	
	if (SM_BLOCK.Succeeded())
		Block->SetStaticMesh(SM_BLOCK.Object);

}

// Called when the game starts or when spawned
void ADirt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirt::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

