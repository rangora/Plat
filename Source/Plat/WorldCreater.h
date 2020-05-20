// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/BasicBlock.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WorldCreater.generated.h"


UCLASS()
class PLAT_API AWorldCreater : public AActor {
	GENERATED_BODY()

public:
	AWorldCreater();
	void CreateHeight(FVector position, int height);
	virtual void BeginPlay() override;

private:
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float noise(float x, float y);
	float octave(int x, int y);


	void CreateTerrain();
	void CreateTreeMap();
	void CreateTree(FVector position, int height);

public:
	enum {
		X = 100,
		Y = 100,
		Z = 100,
		BOUNDARY1 = 40,
		BOUNDARY2 = 70,
		BOUNDARY3 = 100
	};

	FVector _Position;

public:
	ABasicBlock* Dirt;
	ABasicBlock* Grass;
	ABasicBlock* Rock;
	ABasicBlock* Tree;

private:
	ABasicBlock* DeployedBlock;
	float _scalingBias = 2.0f;
	int _octaves = 8;
	int* treeMap = nullptr;
	int _param[512];

	/* Block Count */
	size_t DirtCount;
	size_t GrassCount;
	size_t RockCount;
	size_t LeafCount;
	size_t TreeCount;
};
