// Fill out your copyright notice in the Description page of Project Settings.

#include <iostream>
#include <algorithm>
#include "WorldCreater.h"

void AWorldCreater::PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput) {
    for (int x = 0; x < nWidth; x++) {
        for (int y = 0; y < nHeight; y++) {
            float fNoise = 0.0f;
            float fScale = 1.0f;
            float fScaleAcc = 0.0f; // 누적된 값을 통제하기 위해..

            for (int o = 0; o < nOctaves; o++) {
                int nPitch = nWidth >> o;
                int nSampleX1 = (x / nPitch) * nPitch;
                int nSampleY1 = (y / nPitch) * nPitch;

                int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
                int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

                // linear interpolation
                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
                fScaleAcc += fScale;
                fScale = fScale / fBias;
            }
            fOutput[x * nWidth + y] = fNoise / fScaleAcc;
        }
    }
}

void AWorldCreater::similar_k_means() {
    int index{};
    for (int i = 0; i < MAPWIDTH; i++) {
        for (int j = 0; j < MAPHEIGHT; j++) {
            int sum{};

            index = i * MAPWIDTH + j;

            // boundary check..
            if ((index - MAPWIDTH) < 0 ||
                (index + MAPWIDTH) > MAPWIDTH * MAPHEIGHT ||
                (index % 70) == 0 ||
                (index % 69) == 0) continue;


            sum += _y[index] - _y[index - MAPWIDTH - 1] + _y[index] - _y[index - MAPWIDTH] + _y[index] - _y[index - MAPWIDTH + 1] +
                _y[index] - _y[index - 1] + _y[index] - _y[index + 1] +
                _y[index] - _y[index + MAPWIDTH - 1] + _y[index] - _y[index + MAPWIDTH] + _y[index] - _y[index + MAPWIDTH + 1];

            if (abs(sum) >= 5) {
                if (sum) _y[index]--;
                else    _y[index]++;
            }
        }
    }
}

AWorldCreater::AWorldCreater() {
 	PrimaryActorTick.bCanEverTick = false;	

	_Position = FVector::ZeroVector;
	DirtBlocks = ADirt::StaticClass();
	EarthBlocks = AEarth::StaticClass();

	// random Seed..
	fNoiseSeed2D = new float[MAPWIDTH * MAPHEIGHT];
	fPerlinNoise2D = new float[MAPWIDTH * MAPHEIGHT];
	_y = new int[MAPWIDTH * MAPHEIGHT];

    std::fill_n(_y, MAPWIDTH * MAPHEIGHT, 1);

    for (int i = 0; i < MAPWIDTH * MAPHEIGHT; i++) {
        fNoiseSeed2D[i] = FMath::FRandRange(0.0f, 1.0f);
    }

    PerlinNoise2D(MAPWIDTH, MAPHEIGHT, fNoiseSeed2D, _octaves, _scalingBias, fPerlinNoise2D);

    for (int i = 0; i < MAPWIDTH; i++) {
        for (int j = 0; j < MAPHEIGHT; j++) {
            _y[i * MAPWIDTH + j] += (int)(fPerlinNoise2D[i * MAPWIDTH + j] * 1000.0f / 20.0f);
        }
    }
}


void AWorldCreater::CreateHeight(FVector position, int height) {
    if (height > 60) {
        UE_LOG(LogTemp, Warning, TEXT("if over 60.."));
        height = 60;
    }   
    GetWorld()->SpawnActor<AEarth>(EarthBlocks,
		position + FVector(0.f, 0.f, 100.f * (height - 1)), FRotator::ZeroRotator);
}

void AWorldCreater::BeginPlay() {
	Super::BeginPlay();
	
    //similar_k_means();
    for (int i = 0; i < MAPWIDTH; i++) {
        for (int j = 0; j < MAPHEIGHT; j++) {
            CreateHeight(_Position, _y[i * MAPWIDTH + j]);
            _Position += FVector(0.0f, 100.0f, 0.0f);
        }
        _Position = FVector(_Position.X + 100.0f, 0.0f, 0.0f);
    }
}

void AWorldCreater::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

