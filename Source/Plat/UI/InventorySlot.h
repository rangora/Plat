// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "system/AvatarController.h"
#include "item/InventoryItem.h"
#include "avatar/Avatar.h"
#include "Components/Image.h"
#include "UI/DragDropSlot.h"
#include "Components/TextBlock.h"
#include "InventorySlot.generated.h"


/**
 *
 */

#define QUICKSTART 12
#define QUICKEND   15


UCLASS()
class PLAT_API UInventorySlot : public UUserWidget {
	GENERATED_BODY()

public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void ChangeItemCount(int num);

	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	/* Call when you deteched the button and Outoperation should be not nullptr. */
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
		bool SetNewItem(FInventoryItem NewItem);

	void Refresh();

	bool UseItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Allocatable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Index; // Allocated in InitInventory()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* ThumbnailImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FInventoryItem ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInventorySlot* DragImageWidget;
};
