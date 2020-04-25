// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"

FInventoryItem::FInventoryItem() {
	ItemName = FText::FromString("None");
	Type = EItemType::NONE;
	ItemID = "0";
	Weight = 0;
	Value = 0;
	Thumbnail = LoadObject<UTexture2D>(NULL, TEXT("/Game/resources/UI/InventorySlot.InventorySlot"), NULL, LOAD_None, NULL);
	Description = FText::FromString("None");
}

FInventoryItem::FInventoryItem(const FInventoryItem& Item) {
	ItemName = Item.ItemName;
	Type = Item.Type;
	ItemID = Item.ItemID;
	Weight = Item.Weight;
	Value = Item.Value;
	Thumbnail = Item.Thumbnail;
	Description = Item.Description;
}

void FInventoryItem::Clear() {
	ItemName = FText::FromString("None");
	Type = EItemType::NONE;
	ItemID = "0";	
	Weight = 0;
	Value = 0;
	Thumbnail = LoadObject<UTexture2D>(NULL, TEXT("/Game/resources/UI/InventorySlot.InventorySlot"), NULL, LOAD_None, NULL);
	Description = FText::FromString("None");

}

void FInventoryItem::SetThisItem(const FInventoryItem& Item) {
	ItemName = Item.ItemName;
	Type = Item.Type;
	ItemID = Item.ItemID;
	Weight = Item.Weight;
	Value = Item.Value;
	Thumbnail = Item.Thumbnail;
	Description = Item.Description;
}