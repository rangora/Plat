// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSlot.h"
#include "item/CBaseItemData.h"

UBaseSlot::UBaseSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ItemType = EItemType::NONE;
	DefaultTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/resources/UI/InventorySlot.InventorySlot"), NULL, LOAD_None, NULL);
	CurrentTexture = DefaultTexture;
}

bool UBaseSlot::SetThisTextureToThumbnail(UTexture2D* Texture) {
	if (!IsValid(Texture))
		return false;

	ThumbnailImage->SetBrushFromTexture(Texture);
	return true;
}

void UBaseSlot::Clear() {
	ItemID = "None";
	ItemType = EItemType::NONE;
	ItemName = "None";
	Allocatable = true;
	Count = 0;
	CurrentTexture = DefaultTexture;
}

void UBaseSlot::Refresh() {}

bool UBaseSlot::UseItem() { return true; }