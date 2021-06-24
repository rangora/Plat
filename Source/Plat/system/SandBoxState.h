// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "Serialization/StructuredArchive.h"
#include "item/CBaseItemData.h"
#include "SandBoxState.generated.h"

/**
 *
 */

typedef TPair<int, FString> BlockData;
class AWorldCreater;
struct FRecipeData;

UCLASS()
class PLAT_API ASandBoxState : public AGameStateBase {
	GENERATED_BODY()

public:
	ASandBoxState();

	virtual void BeginPlay() override;

	AWorldCreater* GetWorldCreater();
	UDataTable* GetBaseDB() const;
	UDataTable* GetBlockDB() const;
	UDataTable* GetEquipmentDB() const;

	TArray<FRecipeData*> GetItemRecipes(FString StringBits);

public:
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BaseDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* EquipmentDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BlockDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* RecipeDB;

	TArray<FRecipeData*> RecipeTable;

	UPROPERTY()
		AWorldCreater* WorldCreater;
};
