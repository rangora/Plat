// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/BaseUI.h"
#include "UI/QuickSlot.h"
#include "ScreenUI.generated.h"

/**
 *
 */
UCLASS()
class PLAT_API UScreenUI : public UBaseUI {
	GENERATED_BODY()

public:
	UScreenUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void LinkSlot(int index, class UInventorySlot* Slot);
	void SetUsingIndex(int index);
	int GetUsingIndex();

	UFUNCTION(BlueprintImplementableEvent)
		void InitQuickSlots();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class UQuickSlot*> QuickSlots;

private:
	//enum { QuickSlotSize = 8 };

	int SlotSize;
	int usingIndex;
};
