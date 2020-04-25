// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "UI/BaseUI.h"
#include "PlayerInventory.generated.h"

/**
 *
 */

class UInventorySlot;

UCLASS()
class PLAT_API UPlayerInventory : public UBaseUI {
	GENERATED_BODY()

public:

	UPlayerInventory(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	int FindItemInSlots(FName ItemID);

	bool SwapSlot(int leftIndex, int rightIndex);

	UFUNCTION(BlueprintImplementableEvent)
		void InitInventory();

	UFUNCTION(BlueprintCallable)
		int FindEmptySlot();

	bool RefreshQuickSlots();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UInventorySlot*> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UInventorySlot> SlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlotSize;
};
