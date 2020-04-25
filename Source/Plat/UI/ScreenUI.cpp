// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenUI.h"

UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
	SlotSize = 4;
	usingIndex = -1;
}

void UScreenUI::LinkSlot(int index, UInventorySlot* Slot) {
	if (IsValid(QuickSlots[index]) && IsValid(Slot))
		QuickSlots[index]->InitQuickSlots(Slot);
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("%d didn't init"), index));
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
