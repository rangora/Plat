// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CRecipeData.generated.h"

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SlotBits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName OutputItem;
};