// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarHUD.h"

AAvatarHUD::AAvatarHUD() {
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(
		TEXT("/Game/UMG/GameHUD.GameHUD_C"));

	HUDWidgetClass = UI_HUD.Class;
}

void AAvatarHUD::DrawHUD() {
	Super::DrawHUD();

	DrawLine(1, 20, 100, 20, FLinearColor::Blue);
	DrawText(TEXT("SimpleText.."), FLinearColor::Blue, 0, 0, hud_font);
}

void AAvatarHUD::BeginPlay() {
	Super::BeginPlay();

	if (HUDWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	
		if (CurrentWidget)
			CurrentWidget->AddToViewport();
	}
}
