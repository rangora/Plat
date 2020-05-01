// Fill out your copyright notice in the Description page of Project Settings.


#include "CBaseItemData.h"

FBaseItemData::FBaseItemData() {
	Clear();
}

FBaseItemData::FBaseItemData(const FBaseItemData& Item) {
	ItemName = Item.ItemName;
	ItemType = Item.ItemType;
	ItemID = Item.ItemID;
	Thumbnail = Item.Thumbnail;
}

void FBaseItemData::Clear() {
	ItemName = FText::FromString("None");
	ItemType = EItemType::NONE;
	ItemID = "0";
	Thumbnail = LoadObject<UTexture2D>(NULL, TEXT("/Game/resources/UI/InventorySlot.InventorySlot"), NULL, LOAD_None, NULL);

}

void FBaseItemData::SetThisItem(const FBaseItemData& Item) {
	ItemName = Item.ItemName;
	ItemType = Item.ItemType;
	ItemID = Item.ItemID;
	Thumbnail = Item.Thumbnail;
}
