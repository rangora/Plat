// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
	Allocatable = true;
	Count = -1;
	Index = -1;
	ItemData.Clear();
}

void UInventorySlot::NativeConstruct() {
	Super::NativeConstruct();
	ThumbnailImage->SetBrushFromTexture(ItemData.Thumbnail);
}

void UInventorySlot::ChangeItemCount(int num) {
	Count += num;
	Refresh();
}

void UInventorySlot::Refresh() {
	if (Count > 1) {
		ItemCount->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Count == 1)
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	else {
		ItemData.Clear();
		ItemCount->SetVisibility(ESlateVisibility::Hidden);
	}
	ThumbnailImage->SetBrushFromTexture(ItemData.Thumbnail);
}

bool UInventorySlot::UseItem() {
	EItemType Type = ItemData.Type;

	switch (Type) {
	case EItemType::NONE:
		return false;

	case EItemType::BLOCK:
		Count--;
		Refresh();
		break;
		
	case EItemType::FOOD:
		Count--;
		Refresh();
		break;

	default:
		return false;
	}
	
	if (Count == 0) {
		auto IPlayer = Cast<AAvatar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		auto IController = Cast<AAvatarController>(IPlayer->GetController());
		
		if (IPlayer->Weapon)
			IPlayer->Weapon->Destroy();

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
			DragImageWidget->ItemData.Thumbnail = this->ItemData.Thumbnail;
			DragImageWidget->Allocatable = false;
			oper->DefaultDragVisual = DragImageWidget;

		}	
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragDropSlot* oper = Cast<UDragDropSlot>(InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : end"));

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
		FString::Printf(TEXT("Drag start at %d index"), oper->From->Index));

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
		FString::Printf(TEXT("Drag end at %d index"), this->Index));

	auto ICharacter = Cast<AAvatar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	auto IController = Cast<AAvatarController>(ICharacter->GetController());

	int slot1 = oper->From->Index;
	int slot2 = this->Index;
	int usingIndex = IController->ScreenUIWidget->GetUsingIndex();

	if (!IController->PlayerInventoryWidget->SwapSlot(slot1, slot2))
		return false;

	// If there are any swap slots that bind with quick slot, refresh it.
	if ((QUICKSTART <= slot1 && slot1 <= QUICKEND) ||
		(QUICKSTART <= slot2 && slot2 <= QUICKEND)) {
		IController->PlayerInventoryWidget->RefreshQuickSlots(IController);
	}

	// If player use a item in the swapped slot.
	if (slot1 == usingIndex + QUICKSTART || slot2 == usingIndex + QUICKSTART) {
		if (ICharacter->Weapon != nullptr) {
			IController->ScreenUIWidget->SetUsingIndex(-1);
			ICharacter->Weapon->Destroy();
			ICharacter->Weapon = nullptr;
		}
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

bool UInventorySlot::SetNewItem(FInventoryItem NewItem) {	
	ItemData.SetThisItem(NewItem);
	Count = 1;
	Allocatable = false;
	Refresh();

	return true;
}

