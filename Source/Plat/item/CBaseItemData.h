// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CBaseItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8 {
	NONE		UMETA(DisplayName = "None"),
	EQUIPMENT	UMETA(DisplayName = "Equipment"),
	BLOCK		UMETA(DisplayName = "Block"),
	FOOD		UMETA(DisplayName = "Food")
};


USTRUCT(BlueprintType)
struct FBaseItemData : public FTableRowBase {
	GENERATED_BODY()

public:
	FBaseItemData();
	FBaseItemData(const FBaseItemData& Item);

	virtual void Clear();
	void SetThisItem(const FBaseItemData& Item);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;
};