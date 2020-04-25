// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "item/InventoryItem.h"
#include "system/AvatarController.h"
#include "system/SandBoxState.h"

UPlayerInventory::UPlayerInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SlotSize = 16;

	static ConstructorHelpers::FClassFinder<UInventorySlot> UI_SLOT_C(
		TEXT("/Game/UMG/BP_InventorySlot.BP_InventorySlot_C"));

	if (UI_SLOT_C.Succeeded())
		SlotClass = UI_SLOT_C.Class;
}

void UPlayerInventory::NativeConstruct() {
	Super::NativeConstruct();
}

int UPlayerInventory::FindItemInSlots(FName ItemID) {
	for (int i = 0; i < Slots.Num(); i++) {
		if (Slots[i]->ItemData.ItemID == ItemID) 
			return i;
	}

	return -1;
}

bool UPlayerInventory::SwapSlot(int leftIndex, int rightIndex) {
	if (!IsValid(Slots[leftIndex]) || !IsValid(Slots[rightIndex]))
		return false;
	
	FInventoryItem TempData(Slots[leftIndex]->ItemData);
	bool TempAllocatable = Slots[leftIndex]->Allocatable;
	int TempCount = Slots[leftIndex]->Count;	

	// Change ItemData struct.
	Slots[leftIndex]->ItemData.SetThisItem(Slots[rightIndex]->ItemData);
	Slots[rightIndex]->ItemData.SetThisItem(TempData);

	// Change extra info.
	Slots[leftIndex]->Allocatable = Slots[rightIndex]->Allocatable;
	Slots[leftIndex]->Count = Slots[rightIndex]->Count;
	Slots[rightIndex]->Allocatable = TempAllocatable;
	Slots[rightIndex]->Count = TempCount;
	

	Slots[leftIndex]->Refresh();
	Slots[rightIndex]->Refresh();

	return true;
}

int UPlayerInventory::FindEmptySlot() {
	for (int i = 0; i < Slots.Num(); i++) {
		// If the slot didn't allocated any item, return the index.
		if (IsValid(Slots[i]))
			if (Slots[i]->Allocatable)
				return i;
	}

	return -1;
}

bool UPlayerInventory::RefreshQuickSlots() {
	//auto IController = Cast<AAvatarController>(Controller);
	int slotSize = IController->ScreenUIWidget->QuickSlots.Num();
	if (slotSize) {
		IController->ScreenUIWidget->QuickSlots[0]->Refresh();
		IController->ScreenUIWidget->QuickSlots[1]->Refresh();
		IController->ScreenUIWidget->QuickSlots[2]->Refresh();
		IController->ScreenUIWidget->QuickSlots[3]->Refresh();

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("In Refreshed..")));
		return true;
	}
	return false;
}
