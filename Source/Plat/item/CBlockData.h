// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemEnumType.h"
#include "CBlockData.generated.h"

USTRUCT(BlueprintType)
struct FBlockData : public FTableRowBase {
	GENERATED_BODY()

public:
	FBlockData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;
};