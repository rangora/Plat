// Fill out your copyright notice in the Description page of Project Settings.

#include "ScreenUI.h"

UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SlotSize = QuickSlotSize;
	usingIndex = -1;
}

void UScreenUI::LinkSlot(int index, UInventorySlot* Slot) {
	if (IsValid(QuickSlots[index]) && IsValid(Slot))
		QuickSlots[index]->InitQuickSlots(Slot);
}

int UScreenUI::GetUsingIndex() {
	return usingIndex;
}

void UScreenUI::SetUsingIndex(int index) {
	usingIndex = index;
}

void UScreenUI::NativeConstruct() {
	Super::NativeConstruct();
}