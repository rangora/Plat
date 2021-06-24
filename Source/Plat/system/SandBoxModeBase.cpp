// Fill out your copyright notice in the Description page of Project Settings.

#include "SandBoxModeBase.h"

ASandBoxModeBase::ASandBoxModeBase() {
	DefaultPawnClass = AAvatar::StaticClass();
	PlayerControllerClass = AAvatarController::StaticClass();
	GameStateClass = ASandBoxState::StaticClass();
}

void ASandBoxModeBase::PostLogin(APlayerController* newPlayer) {
	Super::PostLogin(newPlayer);
}