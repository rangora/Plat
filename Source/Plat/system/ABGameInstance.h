// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBlockData {
	GENERATED_BODY()

public:
	FBlockData() : MaxHP(100.) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
};

UCLASS()
class PLAT_API UABGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UABGameInstance();
	void Init();
	FBlockData* GetBlockData();
	
};
