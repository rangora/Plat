// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBlock.h"
#include "item/AutoPickup.h"

ABasicBlock::ABasicBlock() {
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	BlockStat = CreateDefaultSubobject<UBasicBlockComponent>(TEXT("BlockStat"));
	RootComponent = Cast<USceneComponent>(BlockMesh);

	BlockMesh->SetCollisionProfileName(TEXT("Block"));	

	ItemID = FName("NO_ID");
	Super::Name = "BasicBlock";
}

void ABasicBlock::DropItem() {
	if (Name.Equals("BasicBlock"))
		return;
	else {
		FString BP_ItemPath = "/Game/Blueprints/Auto_" + Name + "." + "Auto_" + Name + "_C";
		UClass* BP_Item = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_ItemPath));

		if (IsValid(BP_Item)) {
			FVector DropLocation = GetActorLocation();
			GetWorld()->SpawnActor<AAutoPickup>(BP_Item,
				DropLocation, FRotator::ZeroRotator);
		}
	}
}

void ABasicBlock::PostInitializeComponents() {
	Super::PostInitializeComponents();

	BlockStat->OnHPIsZero.AddLambda([this]() {
		SetActorEnableCollision(false);
		DropItem();
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
