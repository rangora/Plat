// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxState.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/GameData.GameData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_EquipmentDB(TEXT("/Game/Data/EquipmentData.EquipmentData"));
	ItemDB = BP_ItemDB.Object;
	EquipmentDB = BP_EquipmentDB.Object;
}

UDataTable* ASandBoxState::GetItemDB() const {
	return ItemDB;
}

UDataTable* ASandBoxState::GetEquipmentDB() const {
	return EquipmentDB;
}

FBaseItemData* ASandBoxState::GetItemData(FName ID) {
	int _code = FCString::Atoi(*ID.ToString());
	FBaseItemData* ItemData = nullptr;


	if(_code < 1000)
		ItemData = ItemDB->FindRow<FBaseItemData>(ID, "");

	else if(_code < 10000)
		ItemData = EquipmentDB->FindRow<FBaseItemData>(ID, "");

	return ItemData;
}
