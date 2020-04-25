// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "item/InventoryItem.h"
#include "UI/BaseSlot.h"
#include "UI/DragDropSlot.h"
#include "InventorySlot.generated.h"


/**
 *
 */

#define QUICKSTART 12
#define QUICKEND   15


UCLASS()
class PLAT_API UInventorySlot : public UBaseSlot {
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	void ChangeItemCount(int num);
	//UFUNCTION(BlueprintCallable)
	bool SetNewItem(FInventoryItem NewItem);

	virtual void NativeConstruct() override;
	virtual bool UseItem() override;
	virtual void Refresh() override;
	
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	/* Call when you deteched the button and Outoperation should be not nullptr. */
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Index; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInventorySlot* DragImageWidget;
};
