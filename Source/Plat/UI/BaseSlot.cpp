// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSlot.h"

UBaseSlot::UBaseSlot(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) {
}

void UBaseSlot::Refresh() {}

bool UBaseSlot::UseItem() { return true; }
