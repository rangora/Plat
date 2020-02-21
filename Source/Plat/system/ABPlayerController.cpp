// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

//void AABPlayerController::OnPossess(APawn* aPawn) {
//	Super::OnPossess(aPawn);
//}

void AABPlayerController::BeginPlay() {
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}