// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CBlockData.generated.h"





UENUM(BlueprintType)
enum class EMatch : uint8 {
	NONE,
	SWORD,
	AXE,
	PICKAXE
};

USTRUCT(BlueprintType) 
struct FBlockData : public FTableRowBase {
	GENERATED_BODY()

public:
	FBlockData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;
};