// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/HUD.h"
#include "UserWidget.h"
#include "AvatarHUD.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API AAvatarHUD : public AHUD {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUDFont)
		UFont* hud_font;


	AAvatarHUD();

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

private:
	// Include custom widgets..
	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
		UUserWidget* CurrentWidget;
};
