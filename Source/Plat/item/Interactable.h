// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class PLAT_API AInteractable : public AActor {
	GENERATED_BODY()

public:
	AInteractable();

	UFUNCTION()
		virtual void Interact(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		FString GetInteractText() const;

public:
	UPROPERTY(EditDefaultsOnly)
		FString Name;

	UPROPERTY(EditDefaultsOnly)
		FString Action;
};
