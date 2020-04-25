// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBlock.h"

ABasicBlock::ABasicBlock() {
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	BlockStat = CreateDefaultSubobject<UBasicBlockComponent>(TEXT("BlockStat"));
	RootComponent = Cast<USceneComponent>(BlockMesh);

	BlockMesh->SetCollisionProfileName(TEXT("Block"));	

	ItemID = FName("NO_ID");
	Super::Name = "BasicBlock";
}

void ABasicBlock::PostInitializeComponents() {
	Super::PostInitializeComponents();

	BlockStat->OnHPIsZero.AddLambda([this]() {
		SetActorEnableCollision(false);
		Destroy();
		});
}

float ABasicBlock::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float finalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	BlockStat->SetDamage(finalDamage);
	return finalDamage;
}

bool ABasicBlock::UseItem() {
	return true;
}
