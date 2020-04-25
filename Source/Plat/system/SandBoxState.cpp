// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxState.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/GameData.GameData"));
	ItemDB = BP_ItemDB.Object;
}

UDataTable* ASandBoxState::GetItemDB() const {
	return ItemDB;
}