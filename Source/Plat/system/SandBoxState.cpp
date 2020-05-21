// Fill out your copyright notice in the Description page of Project Settings.

#include "SandBoxState.h"
#include "WorldCreater.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/BaseData.BaseData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_EquipmentDB(TEXT("/Game/Data/EquipmentData.EquipmentData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_BlockDB(TEXT("/Game/Data/BlockData.BlockData"));
	BaseDB = BP_ItemDB.Object;
	EquipmentDB = BP_EquipmentDB.Object;
	BlockDB = BP_BlockDB.Object;
}

void ASandBoxState::BeginPlay() {
	Super::BeginPlay();

	auto WorldCreaterClass = AWorldCreater::StaticClass();
	
	WorldCreater = GetWorld()->SpawnActor<AWorldCreater>(AWorldCreater::StaticClass(),
		FVector(100.f, 100.f, 100.f), FRotator::ZeroRotator);
}

UDataTable* ASandBoxState::GetBaseDB() const {
	return BaseDB;
}

UDataTable* ASandBoxState::GetBlockDB() const {
	return BlockDB;
}

UDataTable* ASandBoxState::GetEquipmentDB() const {
	return EquipmentDB;
}

TArray<BlockData>* ASandBoxState::GetBlockTable(FName ItemID) {
	int id = FCString::Atoi(*ItemID.ToString());

	switch (id) {
	case 1:
		return &DirtTable;
	case 2:
		return &GrassTable;
	case 3:
		return &TreeTable;
	case 5:
		return &RockTable;
	default:
		break;
	}

	return nullptr;
}

AActor* ASandBoxState::GetMetaBlock(FName ItemID) {
	int id = FCString::Atoi(*ItemID.ToString());

	switch (id) {
	case 1:
		return WorldCreater->Dirt;
	case 2:
		return WorldCreater->Grass;
	case 3:
		return WorldCreater->Tree;
	case 5:
		return WorldCreater->Rock;
	default:
		break;
	}

	return nullptr;
}

