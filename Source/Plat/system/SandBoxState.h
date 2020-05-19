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

class AWorldCreater;

UCLASS()
class PLAT_API ASandBoxState : public AGameStateBase {
	GENERATED_BODY()

public:
	ASandBoxState();

	virtual void BeginPlay() override;

	UDataTable* GetBaseDB() const;
	UDataTable* GetBlockDB() const;
	UDataTable* GetEquipmentDB() const;

	/* Using Item */
	bool UseBlockItem(ACharacter* Player, FString BlockName);

public:
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BaseDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* EquipmentDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BlockDB;


	TArray<TPair<int, FString>> BlockTable;


	AWorldCreater* WorldCreater;
};
