// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "BlockStatComponent.h"
#include "GameFramework/Actor.h"
#include "Dirt.generated.h"

UCLASS()
class PLAT_API ADirt : public AActor {
	GENERATED_BODY()
	
public:	
	ADirt();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Block;

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UBlockStatComponent* BlockStat;
};
