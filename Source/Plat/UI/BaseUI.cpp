// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUI.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"


UBaseUI::UBaseUI(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {

	IController = Cast<AAvatarController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	IPlayer = Cast<AAvatar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}