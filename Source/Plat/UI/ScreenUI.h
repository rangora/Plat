// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "avatar/Avatar.h"
#include "UI/QuickSlot.h"
#include "ScreenUI.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API UScreenUI : public UUserWidget {
	GENERATED_BODY()

public:
	UScreenUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitQuickSlots();

	void LinkSlot(int index, class UInventorySlot* Slot);

	int GetUsingIndex();
	void SetUsingIndex(int index);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class UQuickSlot*> QuickSlots;
	//AAvatar* PlayerAvatar;
private:
	int SlotSize;

	int usingIndex;
};
