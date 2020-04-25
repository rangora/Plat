// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"
#include "system/SandBoxState.h"
#include "GameFramework/GameModeBase.h"
#include "SandBoxModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API ASandBoxModeBase : public AGameModeBase {
	GENERATED_BODY()
	
public:
	ASandBoxModeBase();
	virtual void PostLogin(APlayerController* newPlayer) override;
};
