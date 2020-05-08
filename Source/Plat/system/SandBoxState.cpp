// Fill out your copyright notice in the Description page of Project Settings.

#include "SandBoxState.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/BaseData.BaseData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_EquipmentDB(TEXT("/Game/Data/EquipmentData.EquipmentData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_BlockDB(TEXT("/Game/Data/BlockData.BlockData"));
	BaseDB = BP_ItemDB.Object;
	EquipmentDB = BP_EquipmentDB.Object;
	BlockDB = BP_BlockDB.Object;
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