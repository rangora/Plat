// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EngineMinimal.h"
#include "CrackingMesh.generated.h"

UCLASS()
class PLAT_API ACrackingMesh : public AActor {
	GENERATED_BODY()

public:
	ACrackingMesh();

	UStaticMeshComponent* Crack;
};
