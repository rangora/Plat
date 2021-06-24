// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCreater.h"
#include "TreeCreater.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/TransformNonVectorized.h"
#include <time.h>
#include "system/SandBoxState.h"

void AWorldCreater::Init() {
	// new
	FVector StartMapVector{ 50000.f, 50000.f, 0.f };
	GetWorld()->SpawnActor<AWorldMap>(AWorldMap::StaticClass(), StartMapVector, FRotator::ZeroRotator);
	

	// Init TileMap.
	for (int i = 0; i < MAXSIZE; i++) {
		for (int j = 0; j < MAXSIZE; j++) {
			TileMap[i][j] = TileState::EMPTYFILE;
		}
	}

	// Init rand.
	if (_randSeed != -1) {
		FMath::SRandInit(_randSeed);
	}
	for (int i = 0; i < 512; i++) {
		_param[i] = FMath::RandRange(1, 255);
	}

	
	SpawnAndSetMap(StartMapVector, TileState::MAPCREATE);

	float dx[8]{ 10000.f, 0.f, -10000.f, 0.f, 10000.f, 10000.f, -10000.f, -10000.f };
	float dy[8]{ 0.f, 10000.f, 0.f, -10000.f, -10000.f, 10000.f, -10000.f, 10000.f };

	for (int i = 0; i < 8; i++) {
		FVector CollisionVector{ StartMapVector.X + dx[i], StartMapVector.Y + dy[i], 0.f };
		SpawnAndSetMap(CollisionVector, TileState::MAPCREATE);

	}
	
}

void AWorldCreater::AutoMapLoader(FVector PlayerVec, TArray<AWorldMap*> NearMaps) {
	if (bLock == false) {
		bLock = true;

		for (auto Map : NearMaps) {
			auto CurrentMap = GetMap(PlayerVec);
			auto CurrentMapVector = CurrentMap->MapLocation;
			auto MapVector = Map->MapLocation;

			if (CurrentMap != nullptr) {


				ExpandNewMap(MapVector);

				// Create terrain	
				Map->RemoveCollisionMesh();

				auto aTask = new MapCreateTask(Map);

				// TEST
				clock_t start, end;
				double result;

				start = clock();
				aTask->DoWorkMain();
				end = clock();
				result = (double)(end - start);

				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
					FString::Printf(TEXT("time:%f"), (result / CLOCKS_PER_SEC)));

				Maps.Add(Map);
				delete aTask;
			}
		}
		
		for (auto ValidMap : Maps) {
				FVector ValidMapVector = ValidMap->MapLocation;
			
				float MapDistance = GetDistance(ValidMapVector, PlayerVec);


				if (FMath::Abs(MapDistance) >= DELETEDISTANCE) {
					Maps.Remove(ValidMap);

					if (ValidMap->IsValidLowLevel()) {
						ValidMap->Clear();
						ValidMap->Destroy();
						ValidMap = nullptr;
						GetWorld()->ForceGarbageCollection();

						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
							FString::Printf(TEXT("Map Delete..")));
					}

					

					SpawnAndSetMap(ValidMapVector, TileState::COLIISION);
					FPlatformProcess::Sleep(0.01f);

					
				}
			}
		
		bLock = false;
	}
}

void AWorldCreater::AutoUndergroundCreater(TArray<APartial*> Partials) {

	if (bLock == false) {
		for (auto Partial : Partials) {
			int mapIndex = GetMapIndex(Partial->GetActorLocation());

			if(mapIndex >= 0)
				Maps[mapIndex]->CreateUnderground(Partial);
		}
	}
}

void AWorldCreater::ExpandNewMap(FVector MapVector) {
	auto MapXY = GetTileXY(MapVector);
	int dx[4]{ 0,0,-1,1 };
	int dy[4]{ 1,-1,0,0 };

	for (int i = 0; i < 4; i++) {
		TileXY DirectionXY{MapXY.first + dx[i], MapXY.second + dy[i]};

		if (TileMap[DirectionXY.first][DirectionXY.second] == TileState::EMPTYFILE) {
			FVector TargetVector{ DirectionXY.first * 10000.f, DirectionXY.second * 10000.f, 0.f };

			SpawnAndSetMap(TargetVector, TileState::COLIISION);
		}
	}
}

AWorldMap* AWorldCreater::GetMap(FVector Vec) {
	FVector Input = { FMath::FloorToFloat((Vec.X / mapSize)) * mapSize,
					  FMath::FloorToFloat((Vec.Y / mapSize))* mapSize,
					  0.f };


	for (auto iter : Maps) {
		if (iter->MapLocation.Equals(Input)) {
			return iter;
		}
	}
	return nullptr;
}

int AWorldCreater::GetMapIndex(FVector Vec) {
	int index = -1;
	FVector Input = { FMath::FloorToFloat((Vec.X / mapSize)) * mapSize,
					  FMath::FloorToFloat((Vec.Y / mapSize)) * mapSize,
					  0.f };

	for (int i = 0; i < Maps.Num(); i++) {
		if (Maps[i]->MapLocation.Equals(Input)) {
			index = i;
			break;
		}
	}

	return index;
}

TileXY AWorldCreater::GetTileXY(FVector Vec) {
	int _x = FMath::FloorToFloat((Vec.X / mapSize));
	int _y = FMath::FloorToFloat((Vec.Y / mapSize));

	TileXY NewTileXY{ _x, _y };

	return NewTileXY;
}

void AWorldCreater::SetMapFileState(TileXY pTileXY, TileState State) {
	if (pTileXY.first < 0 || pTileXY.second < 0)
		return;

	TileMap[pTileXY.first][pTileXY.second] = State;
}

APartial* AWorldCreater::GetAPartialCurrentMap(FVector Vec, int mapIndex) {
	auto Partial = Maps[mapIndex]->GetPartial(Vec);

	if (Partial != nullptr)
		return Partial;

	return nullptr;
}

void AWorldCreater::SpawnAndSetMap(FVector Location, TileState State) {
	AWorldMap* NewMap = nullptr;

	switch (State) {
		case TileState::MAPCREATE:
			NewMap = GetWorld()->SpawnActor<AWorldMap>(AWorldMap::StaticClass(), Location, FRotator::ZeroRotator);
			NewMap->RemoveCollisionMesh();
			NewMap->Init(_param, Location);
			NewMap->CreateTerrain();
			Maps.Add(NewMap);
			
			break;

		case TileState::COLIISION:
			NewMap = GetWorld()->SpawnActor<AWorldMap>(AWorldMap::StaticClass(), Location, FRotator::ZeroRotator);
			NewMap->Init(_param, Location);

			break;

		default:
			break;
	}
	
	auto NewTileXY = GetTileXY(Location);
	SetMapFileState(NewTileXY, State);
}

float AWorldCreater::GetDistance(FVector LeftVec, FVector RightVec) {
	return FMath::Sqrt(FMath::Pow((RightVec.X - LeftVec.X), 2) + FMath::Pow((RightVec.Y - LeftVec.Y), 2));
}

AWorldCreater::AWorldCreater() {
	PrimaryActorTick.bCanEverTick = false;
	mapLoadThreshold = 7000.f;
	mapSize = 10000.f;
	_randSeed = -1;
}

void AWorldCreater::BeginPlay() {
	Super::BeginPlay();

	Init();
}