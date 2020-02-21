// Fill out your copyright notice in the Description page of Project Settings.


#include "Dirt.h"

// Sets default values
ADirt::ADirt() {
	PrimaryActorTick.bCanEverTick = false;

	Block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DIRT"));
	RootComponent = Block;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BLOCK(
		TEXT("/Game/resources/blocks/dirt.dirt"));
	BlockStat = CreateDefaultSubobject<UBlockStatComponent>(TEXT("BLOCKSTAT"));



	if (SM_BLOCK.Succeeded())
		Block->SetStaticMesh(SM_BLOCK.Object);

	
	Block->SetCollisionProfileName(TEXT("Block"));
}

void ADirt::BeginPlay() {
	Super::BeginPlay();	
}


void ADirt::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ADirt::PostInitializeComponents() {
	Super::PostInitializeComponents();

	BlockStat->OnHPIsZero.AddLambda([this]()->void {
		//ABLOG(Warning, TEXT("OnHPIsZero"));
		SetActorEnableCollision(false);
		Destroy();
		});
}

float ADirt::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("DIRT_BLOCK Took Damage"));

	BlockStat->SetDamage(FinalDamage);
	return FinalDamage;
}

