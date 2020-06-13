// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemEnumType.h"
#include "CBaseItemData.generated.h"

USTRUCT(BlueprintType)
struct FBaseItemData : public FTableRowBase {
	GENERATED_BODY()

public:
	FBaseItemData();

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