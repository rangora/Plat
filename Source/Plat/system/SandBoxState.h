// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "SandBoxState.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API ASandBoxState : public AGameStateBase {
	GENERATED_BODY()
	
public:
	ASandBoxState();
	
	UDataTable* GetItemDB() const;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemDB;
};
