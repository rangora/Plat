// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

UABGameInstance::UABGameInstance() {}



void UABGameInstance::Init() {
	Super::Init();
}

// 500 200 200
void UABGameInstance::CreateWorld() {


}

FBlockData* UABGameInstance::GetBlockData() {
	return new FBlockData();
}