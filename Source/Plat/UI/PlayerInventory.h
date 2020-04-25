// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventorySlot.h"
#include "item/InventoryItem.h"
#include "system/AvatarController.h"
#include "system/SandBoxState.h"
#include "PlayerInventory.generated.h"

/**
 *
 */
UCLASS()
class PLAT_API UPlayerInventory : public UUserWidget {
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

	bool RefreshQuickSlots(APlayerController* Controller);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UInventorySlot*> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UInventorySlot> DragImageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SlotSize;
};
