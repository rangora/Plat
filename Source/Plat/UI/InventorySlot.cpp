// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"
#include "system/SandBoxState.h"
#include "blocks/BasicBlock.h"
#include "UI/DragDropSlot.h"
#include "Item/CBlockData.h"
#include "item/CEquipmentData.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	Allocatable = true;
	Count = -1;
	Index = -1;
}

void UInventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	PlayerInventory = IController->PlayerInventoryWidget;
	SetThisTextureToThumbnail(CurrentTexture);
}

void UInventorySlot::AddItemCount(int num) {
	Count += num;
	Refresh();
}

bool UInventorySlot::RefreshQuickSlot() {
	// Check whether this slot is bind with quick slot or not,
	// if bind, refresh it.
	if (QuickStart <= Index && Index <= QuickEnd) {
		int qIndex = Index - QuickStart;
		IController->ScreenUIWidget->QuickSlots[qIndex]->Refresh();
		return true;
	}
	return false;
}

void UInventorySlot::Refresh() {
	if (Count > 1) {
		ItemCount->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Count == 1)
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	else {
		Clear();
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	}
	SetThisTextureToThumbnail(CurrentTexture);
}

bool UInventorySlot::UseItem() {
	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());

	switch (ItemType) {
	case EItemType::BLOCK:
		if (!IPlayer->DeployBlock(ItemID, ItemName))
			return false;
		break;

	case EItemType::FOOD:
		break;

	case EItemType::EQUIPMENT:

	default:
		return false;
	}

	Count--;
	Refresh();

	if (Count == 0) {
		if (IPlayer->GetWeapon())
			IPlayer->GetWeapon()->Destroy();

		IController->ScreenUIWidget->SetUsingIndex(-1);
	}

	return true;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr) {
		UDragDropSlot* oper = NewObject<UDragDropSlot>();
		oper->From = this;
		OutOperation = oper;

		if (DragImageWidget) {
			DragImageWidget->CurrentTexture = this->CurrentTexture;
			DragImageWidget->Allocatable = false;
			oper->DefaultDragVisual = DragImageWidget;
		}
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* oper = Cast<UDragDropSlot>(InOperation);

	int from = oper->From->Index;
	int to = this->Index;

	// Can't put in a item to the output slot.
	if (to == Output)
		return false;

	NormalSlotSwap(from, to);

	// Get into the recipe slot.
	if ((RecipeStart <= to && to <= RecipeEnd) ||
		(RecipeStart <= from && from <= RecipeEnd)) {
		UpdateRecipeSlots();
		PlayerInventory->AssembleRecipe();
	}

	// Take out from output slot.
	else if (from == Output) {
		DragOutFromOutputSlot();
	}

	return true;
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Right mouse button was clicked
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Right button was clicked"));
	}
	// Left mouse button was clicked
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Left button was clicked"));
		// If the slot was not empty
		if (!Allocatable) {
			// Move to 'NativeOnDragDetected' function through DetectDragIfPressed function.
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
	return reply.NativeReply;
}

FReply UInventorySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;

	return reply.NativeReply;
}

bool UInventorySlot::NormalSlotSwap(int leftIndex, int rightIndex) {
	int usingIndex = IController->ScreenUIWidget->GetUsingIndex();

	if (!IController->PlayerInventoryWidget->SwapSlot(leftIndex, rightIndex))
		return false;

	// If there are any swap slots that bind with quick slot, refresh it.
	if ((QuickStart <= leftIndex && leftIndex <= QuickEnd) ||
		(QuickStart <= rightIndex && rightIndex <= QuickEnd)) {
		IController->PlayerInventoryWidget->RefreshQuickSlots();
	}

	// If player use a item in the swapped slot, disarm the weapon.
	if (leftIndex == usingIndex + QuickStart || rightIndex == usingIndex + QuickStart) {
		if (IPlayer->GetWeapon() != nullptr) {
			IController->ScreenUIWidget->SetUsingIndex(-1);
			IPlayer->DisarmWeapon();
		}
	}

	return true;
}

void UInventorySlot::UpdateRecipeSlots() {
	int recipeIndex = RecipeStart;

	for (int i = RecipeStart; i <= RecipeEnd; i++) {
		PlayerInventory->RecipeSlotIDs[i - RecipeStart] = PlayerInventory->Slots[i]->ItemID;
		PlayerInventory->RecipeSlotFlags[i - RecipeStart] = 1;

		if (PlayerInventory->RecipeSlotIDs[i - RecipeStart].IsEqual("None")) {
			PlayerInventory->RecipeSlotFlags[i - RecipeStart] = 0;
		}
	}
}

void UInventorySlot::DragOutFromOutputSlot() {
	for (int i = RecipeStart; i < RecipeStart + 9; i++) {
		if (!IController->PlayerInventoryWidget->Slots[i]->ItemID.IsEqual("None")) {
			IController->PlayerInventoryWidget->Slots[i]->Count--;
			IController->PlayerInventoryWidget->Slots[i]->Refresh();
		}
	}
}

bool UInventorySlot::SetNewItem(FName ID) {
	auto IState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	FBaseItemData* BaseData = IState->BaseDB->FindRow<FBaseItemData>(ID, "");

	// Set empty slot.
	if (ID.IsEqual("None")) {
		this->Clear();
	}

	// Set valid item.
	else {
		if (BaseData == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
				FString::Printf(TEXT("InValid ID..")));
			return false;
		}

		switch (BaseData->ItemType) {
		case EItemType::EQUIPMENT:
			ItemType = EItemType::EQUIPMENT;
			break;

		case EItemType::BLOCK:
			ItemType = EItemType::BLOCK;
			break;

		case EItemType::NONE:

		default:
			return false;
		}

		CurrentTexture = BaseData->Thumbnail;
		ItemName = BaseData->ItemName.ToString();
		ItemID = ID;
		Allocatable = false;
		Count = 1;
	}

	Refresh();

	return true;
}