// Fill out your copyright notice in the Description page of Project Settings.


#include "FMapWorker.h"
#include "Partial.h"
#include "WorldMap.h"
#include "blocks/BasicBlock.h"



FMapWorker::FMapWorker(AWorldMap* aMap) {
	pThread = FRunnableThread::Create(this, TEXT("MapThread"), 0);
	bRun = true;
	CurrentMap = aMap;
}

FMapWorker::~FMapWorker() {
	bRun = false;
	delete pThread;
	pThread = NULL;
}

bool FMapWorker::Init() {
	return true;
}

uint32 FMapWorker::Run() {

	if (bRun) {
		//FPlatformProcess::Sleep(0.10);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("In Run")));
		if (CurrentMap == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
				FString::Printf(TEXT("No Run")));
			
		}
		else {
			//ThreadCreateTerrain();
			for (size_t i = 0; i < 10000000000; i++);

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
				FString::Printf(TEXT("Job Done")));
		}
		//FPlatformProcess::Sleep(0.10);
	}
	bRun = false;

	return 0;
}

void FMapWorker::Stop() {
	bRun = false;
	StopTaskCounter.Increment();
	pThread->WaitForCompletion();
}

FMapWorker* FMapWorker::InitWrapper(AWorldMap* aMap) {
	FMapWorker* Runnable = new FMapWorker(aMap);
	
	return Runnable;
}

void FMapWorker::ThreadCreateTerrain() {
	FVector CurrentSlice = CurrentMap->MapLocation;
	size_t cnt{};

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {

			ThreadCreateTerrainPartial(FVector{
				CurrentSlice.X + 1000.f * x,
				CurrentSlice.Y + 1000.f * y,
				CurrentSlice.Z + 6000.f }, cnt);
			cnt += 4;
			FPlatformProcess::Sleep(0.11);
		}
	}
}

void FMapWorker::ThreadCreateTerrainPartial(FVector Where, int pivotIndex) {
	APartial* CurrentPartial = CurrentMap->SurfaceSlice[pivotIndex];
	ABasicBlock* CurrentBlock = nullptr;
	TArray<BlockData>* CurrentTable = nullptr;
	Where = CurrentPartial->GetActorLocation();

	float threshold = 11.0f;
	float noiseScale = 20.f;
	float density{};
	int sliceIndex{};
	int preZ{};
	int currentZ{};
	int* blockCount = nullptr;

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			for (int z = 40 - 1; z >= 0; z--) {
				sliceIndex = pivotIndex + z / 10;
				CurrentPartial = CurrentMap->SurfaceSlice[sliceIndex];

				float xnoise = octave(Where.Z / 100.f + z, Where.Y / 100.f + y, 4, noiseScale);
				float ynoise = octave(Where.X / 100.f + x, Where.Y / 100.f + y, 4, noiseScale);
				float znoise = octave(Where.X / 100.f + x, Where.Z / 100.f + z, 4, noiseScale);

				density = xnoise + ynoise + znoise + z;

				if (density < threshold) {
					FVector Local{ Where.X + x * 100.f, Where.Y + y * 100.f, Where.Z + z * 100.f };
					currentZ = z * 100;


					if (preZ - currentZ != 100) {
						CurrentBlock = CurrentPartial->Grass;
						blockCount = &(CurrentPartial->GrassCount);
						CurrentTable = &(CurrentPartial->GrassTable);

						if (x > 0 && x < 9 &&
							y > 0 && y < 9) {

							float prob = float(z) * 0.003f;

							/*if (TreeDistributor(prob)) {
								CreateTree(Local, CurrentPartial);
							}*/
						}
					}
					else {
						CurrentBlock = CurrentPartial->Dirt;
						blockCount = &(CurrentPartial->DirtCount);
						CurrentTable = &(CurrentPartial->DirtTable);
					}


					FString StrTransform = Local.ToString();
					auto BlockTransform = FTransform(Local);

					CurrentBlock->CreateInstance(BlockTransform);

					auto NewBlockData{ BlockData(*blockCount, StrTransform) };
					CurrentTable->Add(NewBlockData);
					(*blockCount)++;

					preZ = currentZ;

				}
			} // z out
		}

		preZ = 0;
		currentZ = 0;
	}

}

float FMapWorker::fade(float t){
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float FMapWorker::noise(float x, float y) {
	int* _param = CurrentMap->_param;

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

float FMapWorker::octave(int x, int y, int numOfOctaves, float noiseScale) {
	float amplitude = 1;
	float sum{};

	for (int i = 0; i < numOfOctaves; i++) {
		sum += noise(x / noiseScale, y / noiseScale) * amplitude;
		amplitude *= 2.0f;
	}
	return sum;
}

