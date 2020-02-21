// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

UABGameInstance::UABGameInstance() {}

void UABGameInstance::Init() {
	Super::Init();
}

FBlockData* UABGameInstance::GetBlockData() {
	return new FBlockData();
}