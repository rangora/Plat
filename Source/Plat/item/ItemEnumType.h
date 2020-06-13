// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemEnumType.generated.h"

UENUM(BlueprintType)
enum class EEQuipmentDetail : uint8 {
	NONE		UMETA(DisplayName = "None"),
	WEAPON		UMETA(DisplayName = "Weapon"),
	UTILITY		UMETA(DisplayName = "Utility")
};

UENUM(BlueprintType)
enum class EMatch : uint8 {
	NONE		UMETA(DisplayName = "None"),
	SWORD		UMETA(DisplayName = "Sword"),
	AXE			UMETA(DisplayName = "Axe"),
	PICKAXE		UMETA(DisplayName = "Pickaxe")
};

UENUM(BlueprintType)
enum class EItemType : uint8 {
	NONE		UMETA(DisplayName = "None"),
	EQUIPMENT	UMETA(DisplayName = "Equipment"),
	BLOCK		UMETA(DisplayName = "Block"),
	FOOD		UMETA(DisplayName = "Food")
};