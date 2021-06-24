// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class PLAT_API ABlock : public AActor {
	GENERATED_BODY()

public:
	ABlock();

public:
	UPROPERTY(EditDefaultsOnly)
		FString Name;

	/* Unique value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
};
