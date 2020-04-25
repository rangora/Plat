// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"

UQuickSlot::UQuickSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void UQuickSlot::InitQuickSlots(UInventorySlot* Slot) {
	if (IsValid(Slot)) {
		LinkedSlot = Slot;
		PointedItem.Thumbnail = LinkedSlot->ItemData.Thumbnail;
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
		Border->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuickSlot::Refresh() {
	if (IsValid(LinkedSlot)) {
		PointedItem.Thumbnail = LinkedSlot->ItemData.Thumbnail;
		ItemCount->SetText(FText::FromString(FString::FromInt(LinkedSlot->Count)));
		if (LinkedSlot->Count > 1) {
			ItemCount->SetVisibility(ESlateVisibility::Visible);
		}
		else if (LinkedSlot->Count == 1)
			ItemCount->SetVisibility(ESlateVisibility::Hidden);
		// If the item is out of order.
		else {
			PointedItem.Clear();
			ItemCount->SetVisibility(ESlateVisibility::Hidden);
			ShowBorder(false);
		}
	}
}

void UQuickSlot::UseItem() {
	if (LinkedSlot->UseItem()) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("QuickSlot USE..")));
		Refresh();
	}
}

void UQuickSlot::ShowBorder(bool isInUsed) {
	if(isInUsed)
		Border->SetVisibility(ESlateVisibility::Visible);
	else 
		Border->SetVisibility(ESlateVisibility::Hidden);
}

