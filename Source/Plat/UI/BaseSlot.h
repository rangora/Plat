// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/BaseUI.h"
#include "item/InventoryItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "BaseSlot.generated.h"


UCLASS()
class PLAT_API UBaseSlot : public UBaseUI {
	GENERATED_BODY()

public:
	UBaseSlot(const FObjectInitializer& ObjectInitializer);

	virtual void Refresh();
	virtual bool UseItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Allocatable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* ThumbnailImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FInventoryItem ItemData;
};
