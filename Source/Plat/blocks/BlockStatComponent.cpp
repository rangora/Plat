// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockStatComponent.h"

// Sets default values for this component's properties
UBlockStatComponent::UBlockStatComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	CurrentHP = 1.;
}

void UBlockStatComponent::SetAttribute() {
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);
	CurrentStatData = ABGameInstance->GetBlockData();
	
	if (nullptr != CurrentStatData) {
		CurrentHP = CurrentStatData->MaxHP;
	}
}


void UBlockStatComponent::SetDamage(float NewDamage) {
	ABCHECK(nullptr != CurrentStatData)
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);

	if (CurrentHP <= 0.0f) {
		OnHPIsZero.Broadcast();
	}
}

// Called when the game starts
void UBlockStatComponent::BeginPlay() {
	Super::BeginPlay();
}

void UBlockStatComponent::InitializeComponent() {
	Super::InitializeComponent();
	SetAttribute();
}


// Called every frame
void UBlockStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

