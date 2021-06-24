// Fill out your copyright notice in the Description page of Project Settings.

#include "SandBoxState.h"
#include "WorldCreater.h"
#include "item/CRecipeData.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/BaseData.BaseData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_EquipmentDB(TEXT("/Game/Data/EquipmentData.EquipmentData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_BlockDB(TEXT("/Game/Data/BlockData.BlockData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_RecipeDB(TEXT("/Game/Data/RecipeData.RecipeData"));

	BaseDB = BP_ItemDB.Object;
	EquipmentDB = BP_EquipmentDB.Object;
	BlockDB = BP_BlockDB.Object;
	RecipeDB = BP_RecipeDB.Object;

	// Init Recipe table.
	FString Contexts;
	RecipeDB->GetAllRows<FRecipeData>(Contexts, RecipeTable);
}

void ASandBoxState::BeginPlay() {
	Super::BeginPlay();

	WorldCreater = GetWorld()->SpawnActor<AWorldCreater>(AWorldCreater::StaticClass(),
		FVector::ZeroVector, FRotator::ZeroRotator);
}

AWorldCreater* ASandBoxState::GetWorldCreater() {
	return WorldCreater;
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

TArray<FRecipeData*> ASandBoxState::GetItemRecipes(FString StringBits) {
	TArray<FRecipeData*> Nominated;

	for (auto iter : RecipeTable) {
		if (iter->SlotBits.Equals(StringBits))
			Nominated.Add(iter);
	}

	return Nominated;
}