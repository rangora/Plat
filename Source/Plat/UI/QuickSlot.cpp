// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"

UQuickSlot::UQuickSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void UQuickSlot::NativeConstruct() {
	Super::NativeConstruct();
}

void UQuickSlot::InitQuickSlots(UInventorySlot* Slot) {
	LinkedSlot = Slot;
	if (IsValid(LinkedSlot)) {
		Refresh();
	}
}

void UQuickSlot::Refresh() {
	if (IsValid(LinkedSlot)) {
		ItemCount->SetText(FText::FromString(FString::FromInt(LinkedSlot->Count)));
		if (LinkedSlot->Count > 1) {
			ItemCount->SetVisibility(ESlateVisibility::Visible);
		}
		else if (LinkedSlot->Count == 1)
			ItemCount->SetVisibility(ESlateVisibility::Hidden);
		// If the item is out of order.
		else {
			ItemCount->SetVisibility(ESlateVisibility::Hidden);
			ShowBorder(false);
		}
		ThumbnailImage->SetBrushFromTexture(LinkedSlot->ItemData.Thumbnail);
	}

}

bool UQuickSlot::UseItem() {
	if (LinkedSlot->UseItem()) {
		Refresh();
		return true;
	}
	return false;
}

void UQuickSlot::ShowBorder(bool isInUsed) {
	if(isInUsed)
		Border->SetVisibility(ESlateVisibility::Visible);
	else 
		Border->SetVisibility(ESlateVisibility::Hidden);
}

