// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "UI/BaseSlot.h"
#include "UI/InventorySlot.h"
#include "QuickSlot.generated.h"

UCLASS()
class PLAT_API UQuickSlot : public UBaseSlot {
	GENERATED_BODY()

public:
	UQuickSlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void Refresh() override;
	virtual bool UseItem() override;

	/* This function will be called in CreateInventory() of AvatarController. */
	void InitQuickSlots(class UInventorySlot* Slot);
	void ShowBorder(bool isInUsed);

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UImage* Border;

	UPROPERTY(VisibleAnywhere)
		UInventorySlot* LinkedSlot;
};
