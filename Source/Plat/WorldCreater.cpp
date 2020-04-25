// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCreater.h"
#include <iostream>
#include <algorithm>

float AWorldCreater::fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float AWorldCreater::lerp(float t, float a, float b) {
	return a + t * (b - a);
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

	int A = _s[X] + Y;
	int B = _s[X + 1] + Y;

#define xFlags 0x46552222
#define yFlags 0x2222550A

	hash = (_s[_s[A]] & 0xF) << 1;
	// Grad(p[p[A]], x, y)
	double g22 = (((xFlags >> hash) & 3) - 1) * x + (((yFlags >> hash) & 3) - 1) * y;
	hash = (_s[_s[B]] & 0xF) << 1;
	// Grad(p[p[A]], x - 1, y)
	double g12 = (((xFlags >> hash) & 3) - 1) * (x - 1) + (((yFlags >> hash) & 3) - 1) * y;
	double c1 = g22 + u * (g12 - g22);

	hash = (_s[_s[A + 1]] & 0xF) << 1;
	// Grad(p[p[A + 1], x, y - 1)
	double g21 = (((xFlags >> hash) & 3) - 1) * x + (((yFlags >> hash) & 3) - 1) * (y - 1);
	hash = (_s[_s[B + 1]] & 0xF) << 1;
	// Grad(p[p[A + 1], x - 1, y - 1)
	double g11 = (((xFlags >> hash) & 3) - 1) * (x - 1) + (((yFlags >> hash) & 3) - 1) * (y - 1);
	double c2 = g21 + u * (g11 - g21);

	return c1 + v * (c2 - c1);
}

int AWorldCreater::octave(int x, int y) {
	float amplitude = 1, freq = 1;
	float sum{};

	for (int i = 0; i < 8; i++) {
		sum += noise(x * freq, y * freq) * amplitude;
		amplitude *= 2.0f;
		freq *= 0.5f;
	}
	return floor(floor(sum) / 4) + 1;
}

void AWorldCreater::similar_k_means() {
	int index{};
	int adjacent[8];
	int threshold = 4;

	for (int i = 0; i < MAPWIDTH; i++) {
		for (int j = 0; j < MAPHEIGHT; j++) {
			int key[8]{};
			int value[8]{};
			int correct{};
			int sum{};

			index = i * MAPWIDTH + j;

			// boundary check..
			if ((index - MAPWIDTH) < 0 ||
				(index + MAPWIDTH) > MAPWIDTH * MAPHEIGHT ||
				(index % MAPWIDTH) == 0 ||
				(index % MAPWIDTH - 1) == 0) continue;

			// caculate..
			adjacent[0] = _y[index - MAPWIDTH - 1];
			adjacent[1] = _y[index - MAPWIDTH];
			adjacent[2] = _y[index - MAPWIDTH + 1];
			adjacent[3] = _y[index - 1];
			adjacent[4] = _y[index + 1];
			adjacent[5] = _y[index + MAPWIDTH - 1];
			adjacent[6] = _y[index + MAPWIDTH];
			adjacent[7] = _y[index + MAPWIDTH + 1];;

			/*
			값으로 갯수를 찾아감.(깥은 인덱스 사용..)
			key[i] : i번째 height의 값.
			value[i] : i번째 height의 갯수.
			*/
			for (int k = 0; k < 8; k++) {
				for (int z = 0; z < 8; z++) {
					if (key[z] == adjacent[k]) {
						value[z]++;
						break;
					}
					else if (key[z] == 0) {
						key[z] = adjacent[k];
						value[z]++;
						break;
					}
				}
			}

			for (int k = 0; k < 8; k++) {
				if (threshold < value[k])
					correct = key[k];
			}

			if (correct)
				_y[index] = correct;
		}
	}
}

AWorldCreater::AWorldCreater() {
	PrimaryActorTick.bCanEverTick = false;
	float x{};
	float y{};
	
	_Position = FVector::ZeroVector;
	//DirtBlocks = ADirt::StaticClass();
	//EarthBlocks = AEarth::StaticClass();
	//Blocks = ABlock::StaticClass();

	_y = new int[MAPWIDTH * MAPHEIGHT];
	//std::fill_n(_y, MAPWIDTH * MAPHEIGHT, 1);

	for (int i = 0; i < 512; i++)
		_s[i] = FMath::RandRange(0, 255);

	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			x = (float)j / (float)MAPWIDTH;
			y = (float)i / (float)MAPHEIGHT;

			_y[i * MAPHEIGHT + j] = octave(j, i);
		}
	}
}

void AWorldCreater::CreateHeight(FVector position, int height) {

	//GetWorld()->SpawnActor<AEarth>(EarthBlocks,
	//	position + FVector(0.f, 0.f, 100.f * (height - 1)), FRotator::ZeroRotator);

	//GetWorld()->SpawnActor<ADirt>(DirtBlocks,
	//	position + FVector(0.f, 0.f, 100.f * (height - 2)), FRotator::ZeroRotator);

	FString BP_DirtPath = "Game/Blueprints/BP_Dirt.BP_Dirt_C";
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

	similar_k_means();
	for (int i = 0; i < MAPWIDTH; i++) {
		for (int j = 0; j < MAPHEIGHT; j++) {
			CreateHeight(_Position, _y[i * MAPWIDTH + j]);
			_Position += FVector(0.0f, 100.0f, 0.0f);
		}
		_Position = FVector(_Position.X + 100.0f, 0.0f, 0.0f);
	}
}