// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Earth.generated.h"

UCLASS()
class PLAT_API AEarth : public AActor {
	GENERATED_BODY()
	
public:	
	AEarth();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Block;

};
