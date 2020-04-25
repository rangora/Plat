// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBlockComponent.h"


UBasicBlockComponent::UBasicBlockComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	currentHP = maxHP = 100.f;
	
}


// Called when the game starts
void UBasicBlockComponent::BeginPlay() {
	Super::BeginPlay();
}


void UBasicBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBasicBlockComponent::SetDamage(float NewDamage) {
	currentHP = FMath::Clamp<float>(currentHP - NewDamage, 0.f, maxHP);

	if (currentHP <= 0.f) {
		OnHPIsZero.Broadcast();
	}
}

