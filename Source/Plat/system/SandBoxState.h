// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "item/CBaseItemData.h"
#include "SandBoxState.generated.h"

/**
 * 
 */

UCLASS()
class PLAT_API ASandBoxState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	ASandBoxState();
	
	UDataTable* GetBaseDB() const;
	UDataTable* GetBlockDB() const;
	UDataTable* GetEquipmentDB() const;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BaseDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* EquipmentDB;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* BlockDB;
};
