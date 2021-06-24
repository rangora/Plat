// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/BaseSlot.h"
#include "InventorySlot.generated.h"

/**
 *
 */

UCLASS()
class PLAT_API UInventorySlot : public UBaseSlot {
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	void AddItemCount(int num);
	bool RefreshQuickSlot();
	bool SetNewItem(FName ID);

	virtual void NativeConstruct() override;
	virtual bool UseItem() override;
	virtual void Refresh() override;

	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	/* Call when you deteched the button and Outoperation should be not nullptr. */
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/* Slot interaction. */
	bool NormalSlotSwap(int leftIndex, int rightIndex);
	void UpdateRecipeSlots();

	/* Recipe slot relation. */
	void DragOutFromOutputSlot();

public:
	class UPlayerInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInventorySlot* DragImageWidget;
};
