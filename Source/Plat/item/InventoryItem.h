// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Actor.h"
#include "item/CBaseItemData.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

/** This class represents the picked up items in the player¡¯s inventory.
 * Also if a rarity system (as seen in World of Warcraft, Borderlands, Diablo...) is wanted a property can be added for that.
 */

UENUM(BlueprintType)
enum class EItemType_2 : uint8 {
	NONE	UMETA(DisplayName = "None"), 
	BLOCK	UMETA(DisplayName = "Block"),
	FOOD	UMETA(DisplayName = "Food")
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FBaseItemData {
	GENERATED_BODY()

public:
	FInventoryItem();
	FInventoryItem(const FInventoryItem& Item);
	
	virtual void Clear() override;
	void SetThisItem(const FInventoryItem& Item);

public:
};
