// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <utility>
#include "Plat.h"
#include "blocks/BasicBlock.h"
#include "Partial.h"
#include "WorldMap.h"
#include "FMapWorker.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Containers/Queue.h"
#include "GameFramework/Actor.h"
#include "WorldCreater.generated.h"

typedef std::pair<int,int> TileXY;

UCLASS()
class PLAT_API AWorldCreater : public AActor {
	GENERATED_BODY()

	enum class TileState {
		EMPTYFILE = 0,
		COLIISION = 1,
		MAPCREATE = 2
	};

	enum {
		/* Constants about map crater. */
		X = 100,
		Y = 100,

		TOPZ = 100,
		JOINT = 60,

		MAXSIZE = 100,

		/* Delete map threshold. */
		DELETEDISTANCE = 50000
	};

public:
	AWorldCreater();
	virtual void BeginPlay() override;

	void Init();

	/* Player call two loader functions below. */
	void AutoMapLoader(FVector PlayerVec, TArray<AWorldMap*> NearMaps);
	void AutoUndergroundCreater(TArray<APartial*> Partials);

	/* Create new map from input Vector. */
	void ExpandNewMap(FVector MapVector);

	/* Get map access data from input vector. */
	AWorldMap* GetMap(FVector Vec);
	int GetMapIndex(FVector Vec);

	/* Interact with Tile map. */
	TileXY GetTileXY(FVector Vec);
	void SetMapFileState(TileXY pTileXY, TileState State);

	/* Get partial. */
	APartial* GetAPartialCurrentMap(FVector Vec, int mapIndex);


private:
	void SpawnAndSetMap(FVector Location, TileState State);
	float GetDistance(FVector LeftVec, FVector RightVec);

public:
	UPROPERTY()
		TArray<AWorldMap*> Maps;


private:
	int _param[512];
	int _randSeed;
	float mapLoadThreshold;
	float mapSize;
	TileState TileMap[MAXSIZE][MAXSIZE];
	TileXY PlayerTileXY;

	bool bMapLoaded = false;
	bool bLock = false;
	bool bMapLock = false;
};
