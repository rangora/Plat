// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/BasicBlock.h"
#include "GameFramework/Actor.h"
#include "WorldCreater.generated.h"

#define MAPWIDTH  128
#define MAPHEIGHT 128
#define STEP 100

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
	int octave(int x, int y);

	void similar_k_means();	

public:
	FVector _Position;

private:
	float _scalingBias = 1.8f;
	int _octaves = 8;
	int* _y = nullptr;
	int _s[512];

	//UPROPERTY(EditInstanceOnly)
	//	TSubclassOf<class ABlock> Blocks;
	//UPROPERTY(EditInstanceOnly)
	//	TSubclassOf<class ADirt> DirtBlocks;
	//UPROPERTY(EditInstanceOnly)
	//	TSubclassOf<class AEarth> EarthBlocks;
};
