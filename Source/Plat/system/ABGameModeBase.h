// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API AABGameModeBase : public AGameModeBase {
	GENERATED_BODY()

	AABGameModeBase();

private:
	virtual void BeginPlay() override;

public:
	virtual void PostLogin(APlayerController* newPlayer) override;
};
