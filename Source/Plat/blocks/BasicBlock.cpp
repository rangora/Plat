// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBlock.h"
#include "system/SandBoxState.h"
#include "blocks/CrackingMesh.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"
#include "item/AutoPickup.h"

ABasicBlock::ABasicBlock() {
	Super::Name = "BasicBlock";

	MeshInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instance"));
	RootComponent = Cast<USceneComponent>(MeshInstances);

	MeshInstances->SetCollisionProfileName(TEXT("Block"));
	MeshInstances->SetMobility(EComponentMobility::Movable);
	MeshInstances->SetCullDistance(7500.f);

	ItemID = FName("NO_ID");
	Match = EMatch::NONE;
	maxHP = 100.f;
	currentHP = maxHP;
}

ABasicBlock::~ABasicBlock() {
	if (MeshInstances->IsValidLowLevel()) {
		MeshInstances->ConditionalBeginDestroy();
	}
}

void ABasicBlock::DropItem(FVector DropLocation) {
	if (Name.Equals("BasicBlock"))
		return;
	else {
		FString BP_ItemPath = "/Game/Blueprints/Auto_" + Name + "." + "Auto_" + Name + "_C";
		UClass* BP_Item = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_ItemPath));

		if (IsValid(BP_Item)) {
			GetWorld()->SpawnActor<AAutoPickup>(BP_Item,
				DropLocation, FRotator::ZeroRotator);
		}
	}
}

void ABasicBlock::Reset() {
	currentHP = maxHP;
	instanceIndex = -1;
	BlockLocation = FVector{ -1.f, -1.f, -1.f };
	CrackingEffect->Destroy();
}

const FName ABasicBlock::GetItemID() const {
	return ItemID;
}

void ABasicBlock::CreateInstance(FTransform& BlockTransform) {
	MeshInstances->AddInstance(BlockTransform);
}

void ABasicBlock::Breaking() {
	float CrackingValue = (maxHP - currentHP) / 100.f;

	FVector loc = FVector{ BlockLocation.X + 50.f, BlockLocation.Y + 50.f, BlockLocation.Z + 50.f };

	if (!IsValid(CrackingEffect))
		CrackingEffect = GetWorld()->SpawnActor<ACrackingMesh>(ACrackingMesh::StaticClass(), loc, FRotator::ZeroRotator);

	auto Cracking = CrackingEffect->Crack->CreateDynamicMaterialInstance(0);

	if (Cracking != nullptr) {
		Cracking->SetScalarParameterValue(FName("CrackingValue"), CrackingValue);
	}
}