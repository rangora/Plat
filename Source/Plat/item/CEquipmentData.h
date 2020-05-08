// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "item/CBlockData.h"
#include "CEquipmentData.generated.h"

UENUM(BlueprintType)
enum class EEQuipmentDetail : uint8 {
	NONE		UMETA(DisplayName = "None"),
	WEAPON		UMETA(DisplayName = "Weapon"),
	UTILITY		UMETA(DisplayName = "Utility")
};

USTRUCT(BlueprintType)
struct FEquipmentData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AdventageTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AdDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;
};