// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCreater.h"
#include "TreeCreater.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/TransformNonVectorized.h"
#include "system/SandBoxState.h"
#include <iostream>
#include <algorithm>
#include <random>

float AWorldCreater::fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float AWorldCreater::lerp(float t, float a, float b) {
	return a + t * (b - a); // t = amount
}

float AWorldCreater::grad(int hash, float x, float y, float z) {
	int h = hash & 15;
	int u = h < 8 ? x : y;
	int v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float AWorldCreater::noise(float x, float y) {
	int hash{};
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;

	x -= floor(x);
	y -= floor(y);

	// easy curve..
	double u = fade(x);
	double v = fade(y);

	int A = _param[X] + Y;
	int B = _param[X + 1] + Y;

#define xFlags 0x46552222
#define yFlags 0x2222550A

	hash = (_param[_param[A]] & 0xF) << 1;
	// Grad(p[p[A]], x, y)
	double g22 = (((xFlags >> hash) & 3) - 1) * x + (((yFlags >> hash) & 3) - 1) * y;
	hash = (_param[_param[B]] & 0xF) << 1;
	// Grad(p[p[A]], x - 1, y)
	double g12 = (((xFlags >> hash) & 3) - 1) * (x - 1) + (((yFlags >> hash) & 3) - 1) * y;
	double c1 = g22 + u * (g12 - g22); // Interpolation X.

	hash = (_param[_param[A + 1]] & 0xF) << 1;
	// Grad(p[p[A + 1], x, y - 1)
	double g21 = (((xFlags >> hash) & 3) - 1) * x + (((yFlags >> hash) & 3) - 1) * (y - 1);
	hash = (_param[_param[B + 1]] & 0xF) << 1;
	// Grad(p[p[A + 1], x - 1, y - 1)
	double g11 = (((xFlags >> hash) & 3) - 1) * (x - 1) + (((yFlags >> hash) & 3) - 1) * (y - 1);
	double c2 = g21 + u * (g11 - g21); // Interpolation X.

	// Return Y interpolation value.
	return c1 + v * (c2 - c1);
}

float AWorldCreater::octave(int x, int y) {
	float amplitude = 1, freq = 1;
	float sum{};

	for (int i = 0; i < 8; i++) {
		sum += noise(x * freq, y * freq) * amplitude;
		amplitude *= 2.0f;
		freq *= 0.5f;
	}
	return sum;
}

void AWorldCreater::CreateTerrain() {
	FString BP_DirtPath = "/Game/Blueprints/BP_Dirt.BP_Dirt_C";
	FString BP_GrassPath = "/Game/Blueprints/BP_Grass.BP_Grass_C";
	FString BP_RockPath = "/Game/Blueprints/BP_Rock.BP_Rock_C";

	UClass* BP_Dirt = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_DirtPath));
	UClass* BP_Grass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));
	UClass* BP_Rock = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_RockPath));
	//UClass* DeployedBlock = nullptr;

	Dirt = GetWorld()->SpawnActor<ABasicBlock>(BP_Dirt, FVector::ZeroVector, FRotator::ZeroRotator);
	Grass = GetWorld()->SpawnActor<ABasicBlock>(BP_Grass, FVector::ZeroVector, FRotator::ZeroRotator);
	Rock = GetWorld()->SpawnActor<ABasicBlock>(BP_Rock, FVector::ZeroVector, FRotator::ZeroRotator);

	float threshold = 20.0f;
	float noiseScale = 20.f; // grid를 정의. 높을수록 단순해짐.
	float amp = 16.f;
	int preZ{};
	int currentZ;
	int real{};

	std::random_device seed;
	std::default_random_engine eng(seed());
	std::bernoulli_distribution middle(0.6);
	std::bernoulli_distribution top(0.8); // 다 else로 빠지는데?
	
	for (int x = 0; x < X; x++) {
		for (int y = 0; y < Y; y++) {
			for (int z = Z - 1; z >= 0; z--) {
				float xnoise = noise(z / noiseScale, y / noiseScale) * amp;
				float ynoise = noise(x / noiseScale, y / noiseScale) * amp;
				float znoise = noise(x / noiseScale, z / noiseScale) * amp;

				float density = xnoise + ynoise + znoise + z;

				if (density < threshold && density > 15.f) {
					currentZ = z * 100;
					DeployedBlock = Dirt;

					////Check Surface.
					//if (preZ - currentZ != 100) {
					//	DeployedBlock = Grass;
					//}

					//else {
					//	int layer = z / 10;
					//switch (layer) {
					//
					//// Only rock.
					//case 0:
					//case 1:
					//case 3:
					//	DeployedBlock = Rock;
					//	break;

					//case 4:
					//case 5:
					//case 6:
					//case 7:
					//	if (middle(eng)) {
					//		DeployedBlock = Rock;
					//	}
					//	else {
					//		DeployedBlock = Dirt;
					//	}
					//	break;

					//case 8:
					//case 9:
					//	if (top(eng)) {
					//		DeployedBlock = Dirt;
					//	}
					//	else {
					//		DeployedBlock = Rock;
					//	}
					//	break;

					//case 10:
					//	DeployedBlock = Grass;
					//	break;

					//default:
					//	DeployedBlock = Dirt;
					//	break;
					//}				
					//}
					//preZ = currentZ;
					
					FVector Local{ x * 100.f, y * 100.f, z * 100.f };
					auto BlockTransform = FTransform(Local);

					DeployedBlock->CreateInstance(BlockTransform);
					real++;
					
				}
			}
		}
		preZ = 0;
		currentZ = 0;
	}

	////auto size = Dirt->MeshInstances->GetNumMaterials();
	//auto size = Dirt->MeshInstances->GetNumRenderInstances();
	//Dirt->MeshInstances->GetComponentInstanceData();

	////auto iter2 = Dirt->MeshInstances->GetComponentInstanceData().Cast<ABasicBlock>();

	//auto iter = Dirt->MeshInstances->GetComponentInstanceData();
	//auto arr = Dirt->MeshInstances->PerInstanceSMData;

	//Dirt->MeshInstances;

	//auto arrNum = arr.Num();
	//auto item = arr.GetData();
	////auto tt = item->Transform.TransformVector();
	


	//FVector targetVector;
	//arr[10].Transform.TransformVector(targetVector);


	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//	FString::Printf(TEXT("size:%d"), size));
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//	FString::Printf(TEXT("real:%d"), real));
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//	FString::Printf(TEXT("arrNum:%d"), arrNum));
}

void AWorldCreater::TestCreater() {
	FString BP_DirtPath = "/Game/Blueprints/BP_Dirt.BP_Dirt_C";
	FString BP_GrassPath = "/Game/Blueprints/BP_Grass.BP_Grass_C";
	FString BP_RockPath = "/Game/Blueprints/BP_Rock.BP_Rock_C";

	UClass* BP_Dirt = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_DirtPath));
	UClass* BP_Grass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));
	UClass* BP_Rock = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_RockPath));
	//UClass* DeployedBlock = nullptr;

	Dirt = GetWorld()->SpawnActor<ABasicBlock>(BP_Dirt, FVector::ZeroVector, FRotator::ZeroRotator);
	Grass = GetWorld()->SpawnActor<ABasicBlock>(BP_Grass, FVector::ZeroVector, FRotator::ZeroRotator);
	Rock = GetWorld()->SpawnActor<ABasicBlock>(BP_Rock, FVector::ZeroVector, FRotator::ZeroRotator);

	DeployedBlock = Dirt;

	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());

	int cnt{};

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 10; k++) {
				FVector Local{ k * 100.f + 100.f, j * 100.f + 100.f, i * 100.f + 100.f };
				auto BlockTransform = FTransform(Local);
				FString localString = Local.ToString();
				
				DeployedBlock->CreateInstance(BlockTransform);
				
				auto tempPair{ TPair<int, FString>(cnt, localString) };
				CurrentState->BlockTable.Add(tempPair);
				cnt++;
			}

		}
	}

	for (int i = 0; i < 10; i++) {
		FVector Local{100.f + 800.f, i * 100.f + 100.f, 100.f + 100.f };
		auto BlockTransform = FTransform(Local);
		FString localString = Local.ToString();

		DeployedBlock->CreateInstance(BlockTransform);

		auto tempPair{ TPair<int, FString>(cnt, localString) };
		CurrentState->BlockTable.Add(tempPair);
		cnt++;
	}
			
	auto n1 = CurrentState->BlockTable.Num();
	auto n2 = cnt;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
		FString::Printf(TEXT("n1 : %d"), n1));

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
		FString::Printf(TEXT("n2 : %d"), n2));
}

void AWorldCreater::CreateTreeMap() {
}

void AWorldCreater::CreateTree(FVector position, int height) {
	auto Tree = ATreeCreater::StaticClass();

	GetWorld()->SpawnActor<ATreeCreater>(Tree,
		position + FVector(0.f, 0.f, 100.f * height), FRotator::ZeroRotator);
}

AWorldCreater::AWorldCreater() {
	PrimaryActorTick.bCanEverTick = false;
	
	treeMap = new int[X * Y];

	// Set random value.
	for (int i = 0; i < 512; i++) {
		_param[i] = FMath::RandRange(1, 255);
	}

}

void AWorldCreater::CreateHeight(FVector position, int height) {
	FString BP_DirtPath = "/Game/Blueprints/BP_Dirt.BP_Dirt_C";
	FString BP_GrassPath = "/Game/Blueprints/BP_Grass.BP_Grass_C";

	UClass* BP_Dirt = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_DirtPath));
	UClass* BP_Grass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));


	GetWorld()->SpawnActor<ABasicBlock>(BP_Grass,
		position + FVector(0.f, 0.f, 100.f * (height - 1)), FRotator::ZeroRotator);

	GetWorld()->SpawnActor<ABasicBlock>(BP_Dirt,
		position + FVector(0.f, 0.f, 100.f * (height - 2)), FRotator::ZeroRotator);
}

void AWorldCreater::BeginPlay() {
	Super::BeginPlay();

	//CreateTerrain();
	TestCreater();
}