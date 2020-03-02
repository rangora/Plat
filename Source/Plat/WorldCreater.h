// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/Dirt.h"
#include "blocks/Earth.h"
#include "GameFramework/Actor.h"
#include "WorldCreater.generated.h"


#define MAPWIDTH  70
#define MAPHEIGHT 70
#define STEP 100


UCLASS()
class PLAT_API AWorldCreater : public AActor {
	GENERATED_BODY()
	
private:
	float* fNoiseSeed2D = nullptr;
	float* fPerlinNoise2D = nullptr;
	float _scalingBias = 2.0f;
	int _octaves = 7;
	int* _y = nullptr;


	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);

public:	
	FVector _Position;

	AWorldCreater();
	void CreateHeight(FVector position, int height);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly)
		TSubclassOf<class ADirt> DirtBlocks;
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<class AEarth> EarthBlocks;
};
