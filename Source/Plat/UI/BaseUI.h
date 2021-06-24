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
	enum {
		// Size를 제외한 값들은 폐구간 index를 의미함.
		// Inventoryy UI.
		InventorySlotSize = 41,
		QuickStart = 24,
		QuickEnd = 31,
		RecipeStart = 32,
		RecipeEnd = 40,
		Output = 41,

		// Screen UI.
		QuickSlotSize = 8
	};

	AAvatarController* IController;
	AAvatar* IPlayer;
};
