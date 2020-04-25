// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

/** This class represents the picked up items in the player¡¯s inventory.
 * Also if a rarity system (as seen in World of Warcraft, Borderlands, Diablo...) is wanted a property can be added for that.
 */

UENUM(BlueprintType)
enum class EItemType : uint8 {
	NONE	UMETA(DisplayName = "None"), 
	BLOCK	UMETA(DisplayName = "Block"),
	FOOD	UMETA(DisplayName = "Food")
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase {
	GENERATED_BODY()

public:
	FInventoryItem();
	FInventoryItem(const FInventoryItem& Item);
	
	void Clear();
	void SetThisItem(const FInventoryItem& Item);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
};
