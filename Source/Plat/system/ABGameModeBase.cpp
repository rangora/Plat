// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameModeBase.h"
#include "avatar/Avatar.h"
#include "ABPlayerController.h"
#include "UI/AvatarHUD.h"
#include "Blueprint/UserWidget.h"

AABGameModeBase::AABGameModeBase() {
	DefaultPawnClass = AAvatar::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	HUDClass = AAvatarHUD::StaticClass();
}

void AABGameModeBase::BeginPlay() {
	Super::BeginPlay();
}

void AABGameModeBase::PostLogin(APlayerController* newPlayer) {
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(newPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
