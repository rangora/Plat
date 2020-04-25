// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/UserWidget.h"
#include "BaseUI.generated.h"

/**
 * 
 */

class AAvatarController;
class AAvatar;

UCLASS()
class PLAT_API UBaseUI : public UUserWidget {
	GENERATED_BODY()

public:
	UBaseUI(const FObjectInitializer& ObjectInitializer);

public:
	AAvatarController* IController;
	AAvatar* IPlayer;
};
