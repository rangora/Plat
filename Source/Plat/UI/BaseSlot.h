// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI/BaseUI.h"
#include "system/SandBoxState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "BaseSlot.generated.h"

enum class EItemType : uint8;

UCLASS()
class PLAT_API UBaseSlot : public UBaseUI {
	GENERATED_BODY()

public:
	UBaseSlot(const FObjectInitializer& ObjectInitializer);

	bool SetThisTextureToThumbnail(UTexture2D* Texture);
	void Clear();
	virtual void Refresh();
	virtual bool UseItem();

public:
	UPROPERTY(VisibleAnywhere)
		FName ItemID;

	UPROPERTY(VisibleAnywhere)
		EItemType ItemType;

	UPROPERTY(VisibleAnywhere)
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Allocatable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* ThumbnailImage;

	UTexture2D* DefaultTexture;
	UTexture2D* CurrentTexture;
};
