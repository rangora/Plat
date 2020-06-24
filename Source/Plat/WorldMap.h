// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Partial.h"
#include "HAL/RunnableThread.h"
#include "GameFramework/Actor.h"
#include "WorldMap.generated.h"

UCLASS()
class PLAT_API AWorldMap : public AActor {
	GENERATED_BODY()

public:
	AWorldMap();
	~AWorldMap();

	virtual void BeginPlay() override;

	void Init(int* ramdomArray, FVector aMapLocation);
	void SetLocation(FVector Vec);

	float fade(float t);
	float noise(float x, float y);
	float octave(int x, int y, int numOfOctaves, float noiseScale);

	void CreateTerrain();
	void CreateTerrainSlice(int pivotIndex);
	bool ProbDistributor(float prob);
	void CreateTree(FVector Vec);

	void CreateUnderground(APartial* NearPartials);

	

	APartial* GetPartial(FVector Vec);

	void RemoveCollisionMesh();

	/* Thread */
	void RunCreaterTask(AWorldMap* aMap);
	void RunCreaterTaskMain(AWorldMap* aMap);

	/* Clean */
	void Clear();

private:
	void CreateLog(FVector Vec);
	void CreateLeaf(FVector Vec);


public:
	UStaticMeshComponent* CollisionMesh;

	FVector MapLocation;

	UPROPERTY()
		APartial* SurfaceSlice[400];

	UPROPERTY()
		APartial* UndergroundSlice[600];

	int* _param;
	bool bIsInit = false;

private:
	/* Constants for map creating. */
	enum {
		TOPZ = 100,
		JOINT = 60,
		SURFACEZ = 40,

		X = 100,
		Y = 100
	};

	/* Constants for perlin noise. */
	enum {
		// Common value.
		AMPLITUDE = 2,

		// Values about surface.
		STHRESHOLD = 11,
		SNOISESCALE = 20,
		SOCTAVES = 4,
		
		// Values about underground.
		UTHRESHOLD = 25,
		UNOISESCALE = 18,
		UOCATAVES = 6
	};

	int coalNum = 0;
};


 //=========================================//

class MapCreateTask : public FNonAbandonableTask {
public:
	MapCreateTask(AWorldMap* aMap);
	~MapCreateTask();

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(MapCreateTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork();
	void DoWorkMain();


public:
	AWorldMap* OutputMap;
};

