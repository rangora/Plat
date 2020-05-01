// Fill out your copyright notice in the Description page of Project Settings.


#include "CEquipmentData.h"

FEquipmentData::FEquipmentData() {
	Super();
	Clear();
}

FEquipmentData::FEquipmentData(const FEquipmentData& Item) {
	SetThisItem(Item);
}

void FEquipmentData::Clear() {
	Super::Clear();

	DetailType = EEQuipmentDetail::NONE;
	AdventageTarget = "None";
	AdDamage = -1;
	Damage = -1;
}

void FEquipmentData::SetThisItem(const FEquipmentData& Item) {
	ItemName = Item.ItemName;
	ItemType = Item.ItemType;
	ItemID = Item.ItemID;
	Thumbnail = Item.Thumbnail;

	DetailType = Item.DetailType;
	AdventageTarget = Item.AdventageTarget;
	AdDamage = Item.AdDamage;
	Damage = Item.Damage;
}
