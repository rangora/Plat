// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "item/CRecipeData.h"
#include "system/AvatarController.h"
#include "system/SandBoxState.h"

UPlayerInventory::UPlayerInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SlotSize = InventorySlotSize;

	RecipeSlotIDs.Init("None", 9);

	static ConstructorHelpers::FClassFinder<UInventorySlot> UI_SLOT_C(
		TEXT("/Game/UMG/BP_InventorySlot.BP_InventorySlot_C"));

	if (UI_SLOT_C.Succeeded())
		SlotClass = UI_SLOT_C.Class;
}

void UPlayerInventory::NativeConstruct() {
	Super::NativeConstruct();

	bIsFocusable = true;
	SetKeyboardFocus();
}

FReply UPlayerInventory::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyboardEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnKeyDown(MyGeometry, InKeyboardEvent);

	FString Input = InKeyboardEvent.GetKey().ToString();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
		FString::Printf(TEXT("Input : %s"), *Input));

	if (Input.Equals("B")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
		//	FString::Printf(TEXT("Inside")));
		//IController->PlayerInventoryWidget->RemoveFromParent();
		//IController->IsInventoryOpen = false;
		//IController->bShowMouseCursor = false;

		//FInputModeGameOnly Mode;
		//IController->SetInputMode(Mode);
	}

	return reply.NativeReply;
}

int UPlayerInventory::FindItemInSlots(FName ItemID) {
	for (int i = 0; i < Slots.Num(); i++) {
		if (Slots[i]->ItemID == ItemID)
			return i;
	}

	return -1;
}

bool UPlayerInventory::SwapSlot(int leftIndex, int rightIndex) {
	if (!IsValid(Slots[leftIndex]) || !IsValid(Slots[rightIndex]))
		return false;

	FName TempID = Slots[leftIndex]->ItemID;
	EItemType TempType = Slots[leftIndex]->ItemType;
	FString TempName = Slots[leftIndex]->ItemName;
	int TempCount = Slots[leftIndex]->Count;
	bool TempAllocatable = Slots[leftIndex]->Allocatable;
	UTexture2D* TempTexture = Slots[leftIndex]->CurrentTexture;

	Slots[leftIndex]->ItemID = Slots[rightIndex]->ItemID;
	Slots[leftIndex]->ItemType = Slots[rightIndex]->ItemType;
	Slots[leftIndex]->ItemName = Slots[rightIndex]->ItemName;
	Slots[leftIndex]->Count = Slots[rightIndex]->Count;
	Slots[leftIndex]->Allocatable = Slots[rightIndex]->Allocatable;
	Slots[leftIndex]->CurrentTexture = Slots[rightIndex]->CurrentTexture;

	Slots[rightIndex]->ItemID = TempID;
	Slots[rightIndex]->ItemType = TempType;
	Slots[rightIndex]->ItemName = TempName;
	Slots[rightIndex]->Count = TempCount;
	Slots[rightIndex]->Allocatable = TempAllocatable;
	Slots[rightIndex]->CurrentTexture = TempTexture;

	Slots[leftIndex]->Refresh();
	Slots[rightIndex]->Refresh();

	return true;
}

void UPlayerInventory::AssembleRecipe() {
	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	FString StringFlags = "0b";
	FString PlayerIngradients;
	FName ResultID = "None";

	// preprocess data.
	for (auto iter : RecipeSlotFlags) {
		StringFlags += FString::FromInt(iter);
	}

	for (int i = 0; i < RecipeSlotIDs.Num(); i++) {
		if (RecipeSlotIDs[i].IsEqual("None"))
			continue;

		PlayerIngradients += RecipeSlotIDs[i].ToString();
		PlayerIngradients.Append(",");
	}
	PlayerIngradients.RemoveAt(PlayerIngradients.Len() - 1);

	// Get item list matched with slot flags.
	auto Nominated = CurrentState->GetItemRecipes(StringFlags);

	// Find a correct item in the list.
	for (auto iter : Nominated) {
		FString Ingradients = iter->ItemIDs;

		if (Ingradients.Equals(PlayerIngradients))
			ResultID = iter->OutputItem;
	}

	Slots[Output]->SetNewItem(ResultID);
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
	int slotSize = IController->ScreenUIWidget->QuickSlots.Num();
	if (slotSize) {
		for (int i = 0; i < slotSize; i++)
			IController->ScreenUIWidget->QuickSlots[i]->Refresh();
		return true;
	}
	return false;
}