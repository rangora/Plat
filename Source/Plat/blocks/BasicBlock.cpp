// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBlock.h"
#include "system/SandBoxState.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"
#include "item/AutoPickup.h"

ABasicBlock::ABasicBlock() {
	Super::Name = "BasicBlock";

	//MeshInstances = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	BlockStat = CreateDefaultSubobject<UBasicBlockComponent>(TEXT("BlockStat"));
	MeshInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instance"));
	RootComponent = Cast<USceneComponent>(MeshInstances);
	//RootComponent = Cast<USceneComponent>(BlockMesh);

	//BlockMesh->SetCollisionProfileName(TEXT("Block"));
	MeshInstances->SetCollisionProfileName(TEXT("Block"));

	ItemID = FName("NO_ID");
	Match = EMatch::NONE;
	maxHP = 100.f;
}

void ABasicBlock::DropItem(FVector DropLocation) {
	if (Name.Equals("BasicBlock"))
		return;
	else {
		FString BP_ItemPath = "/Game/Blueprints/Auto_" + Name + "." + "Auto_" + Name + "_C";
		UClass* BP_Item = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_ItemPath));

		if (IsValid(BP_Item)) {
			//FVector DropLocation = GetActorLocation();
			GetWorld()->SpawnActor<AAutoPickup>(BP_Item,
				DropLocation, FRotator::ZeroRotator);
		}
	}
}

void ABasicBlock::Restore() {
	BlockStat->Restore();

	auto Meterials = MeshInstances->GetMaterials();
	TArray<UMaterialInstanceDynamic*> Instances;

	for (int i = 0; i < Meterials.Num(); ++i) {
		Instances.Add(MeshInstances->CreateDynamicMaterialInstance(i, Meterials[i]));
		Instances[i]->SetScalarParameterValue(FName("CrackingValue"), 1.0f);
	}
}

void ABasicBlock::BeginPlay() {
	Super::BeginPlay();
}

void ABasicBlock::PostInitializeComponents() {
	Super::PostInitializeComponents();

	BlockStat->OnHPIsZero.AddLambda([this]() {
		SetActorEnableCollision(false);
		DropItem(GetActorLocation());
		Destroy();
		});
}

float ABasicBlock::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float finalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//float CrackingValue = BlockStat->currentHP / 100.f;
	//auto Meterials = MeshInstances->GetMaterials();
	//
	//TArray<UMaterialInstanceDynamic*> Instances;

	//for (int i = 0; i < Meterials.Num(); ++i) {
	//	Instances.Add(MeshInstances->CreateDynamicMaterialInstance(i, Meterials[i]));
	//	Instances[i]->SetScalarParameterValue(FName("CrackingValue"), CrackingValue);
	//}
	BlockStat->SetDamage(finalDamage);
	return finalDamage;
}

const FName ABasicBlock::GetItemID() const {
	return ItemID;
}

void ABasicBlock::CreateInstance(FTransform& BlockTransform) {
	MeshInstances->AddInstance(BlockTransform);
}
