// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "avatar/Avatar.h"
#include "item/InventoryItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ui/InventorySlot.h"
#include "QuickSlot.generated.h"

/** Only have 12 ~ 15 index.
 * 
 */
UCLASS()
class PLAT_API UQuickSlot : public UUserWidget {
	GENERATED_BODY()

public:
	UQuickSlot(const FObjectInitializer& ObjectInitializer);

	void InitQuickSlots(class UInventorySlot* Slot);

	void Refresh();

	void UseItem();

	void ShowBorder(bool isInUsed);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Border;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UInventorySlot* LinkedSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FInventoryItem PointedItem;

private:
};
