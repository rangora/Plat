// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemEnumType.h"
#include "CEquipmentData.generated.h"

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