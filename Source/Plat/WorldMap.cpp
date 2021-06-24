// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMap.h"
#include <random>

AWorldMap::AWorldMap() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Temp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/resources/blocks/PartialMesh.PartialMesh"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Game/resources/blocks/Lumber.Lumber"));

	if (MeshObject.Succeeded())
		CollisionMesh->SetStaticMesh(MeshObject.Object);

	CollisionMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
	CollisionMesh->SetCollisionProfileName(TEXT("BlockQuery"));
}

AWorldMap::~AWorldMap() {
}

void AWorldMap::BeginPlay() {
	Super::BeginPlay();
}

void AWorldMap::Init(int* ramdomArray, FVector aMapLocation) {
	UE_LOG(Plat, Log, TEXT("WorldMap Init"));
	_param = ramdomArray;
	MapLocation = aMapLocation;

	int i = 0;

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			for (int z = 0; z < 4; z++) {
				SurfaceSlice[i] = GetWorld()->SpawnActor<APartial>(APartial::StaticClass(),
					FVector(MapLocation.X + 1000.f * x, 
							MapLocation.Y + 1000.f * y, 
							1000.f * z + 6000.f), FRotator::ZeroRotator);
				i++;
			}
		}
	}
	FPlatformProcess::Sleep(0.01);

	i = 0;
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			for (int z = 0; z < 6; z++) {
				UndergroundSlice[i] = GetWorld()->SpawnActor<APartial>(APartial::StaticClass(),
					FVector(MapLocation.X + 1000.f * x,
							MapLocation.Y + 1000.f * y,
							1000.f * z), FRotator::ZeroRotator);
				i++;
			}
		}
	}
	FPlatformProcess::Sleep(0.01);
	bIsInit = true;
	UE_LOG(Plat, Log, TEXT("WorldMap Init Out"));
}

void AWorldMap::SetLocation(FVector Vec) {
	MapLocation = Vec;
}

float AWorldMap::noise(float x, float y) {
	int hash{};
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;

	x -= floor(x);
	y -= floor(y);

	// easy curve..
	double u = fade(x);
	double v = fade(y);

	int xi = _param[X] + Y;
	int yi = _param[X + 1] + Y;

	double g11 = grad(_param[_param[xi]], x, y);
	double g12 = grad(_param[_param[yi]], x - 1, y);
	double c1 = g11 + u * (g12 - g11); // Interpolation X top.

	double g21 = grad(_param[_param[xi + 1]], x, y - 1);
	double g22 = grad(_param[_param[yi + 1]], x - 1, y - 1);
	double c2 = g21 + u * (g22 - g21); // Interpolation X bottom.

	float gg = c1 + v * (c2 - c1);

	//GEngine->AddOnScreenDebugMessage(-1, 13.f, FColor::Red, FString::Printf(TEXT("v:%f"), gg));

	// Return Y interpolation value.
	return c1 + v * (c2 - c1);
}

float AWorldMap::fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float AWorldMap::grad(int hash, double x, double y) {
	int xFlag = 0x24552A2F;
	int yFlag = 0x1222A301;

	int h = (hash & 0xF);
	// 해쉬 결과가 0인 것에 대해서 음수로 만듬.
	double output = (((xFlag >> h) & 3) - 1) * x + (((yFlag >> h) & 3) - 1) * y;

	return output;
}

float AWorldMap::MyPerlinNoise(int x, int y, int numOfOctaves, float noiseScale) {
	float amplitude = 2;
	float sum{};

	amplitude = FMath::Pow(2, numOfOctaves);
	sum += noise(x / noiseScale, y / noiseScale) * amplitude;

	return sum;
}

void AWorldMap::CreateWorld() {
	//ABasicBlock* CurrentBlock = nullptr;
	//float density{};
	//int sliceIndex{};
	//int preZ{};
	//int currentZ{};
	//int* blockCount = nullptr;

	//for (int x = 0; x < worldX; x++) {
	//	for (int y = 0; y < worldY; y++) {
	//		for (int z = 40 - 1; z >= 0; z--) {

	//			float xnoise = MyPerlinNoise(z, y, SOCTAVES, SNOISESCALE);
	//			float ynoise = MyPerlinNoise(x, y, SOCTAVES, SNOISESCALE);
	//			float znoise = MyPerlinNoise(x, z, SOCTAVES, SNOISESCALE);
	//			density = xnoise + ynoise + znoise + z;

	//			// TEST
	//			/*float xnoise = FMath::PerlinNoise2D(FVector2D{ (Where.Z / 100.f + z) / 20.f, (Where.Y / 100.f + y) / 20.f}) * 12.f;
	//			float ynoise = FMath::PerlinNoise2D(FVector2D{ (Where.X / 100.f + x) / 20.f, (Where.Y / 100.f + y) / 20.f}) * 12.f;
	//			float znoise = FMath::PerlinNoise2D(FVector2D{ (Where.X / 100.f + x) / 20.f, (Where.Z / 100.f + z) / 20.f}) * 12.f;
	//			*/

	//			if (density < STHRESHOLD) {
	//				FVector Local{ x * 100.f,y * 100.f,z * 100.f };
	//				currentZ = z * 100;


	//				if (preZ - currentZ != 100) {
	//					CurrentBlock = CurrentPartial->Grass;
	//					blockCount = &(CurrentPartial->GrassCount);
	//					CurrentTable = &(CurrentPartial->GrassTable);

	//					if (x > 0 && x < 9 &&
	//						y > 0 && y < 9) {

	//						float prob = float(z) * 0.003f;

	//						/*if (ProbDistributor(prob)) {
	//							CreateTree(Local);
	//						}*/
	//					}
	//				}
	//				else {
	//					CurrentBlock = CurrentPartial->Dirt;
	//					blockCount = &(CurrentPartial->DirtCount);
	//					CurrentTable = &(CurrentPartial->DirtTable);
	//				}


	//				FString StrTransform = Local.ToString();
	//				auto BlockTransform = FTransform(Local);

	//				CurrentBlock->CreateInstance(BlockTransform);

	//				auto NewBlockData{ BlockData(*blockCount, StrTransform) };
	//				CurrentTable->Add(NewBlockData);
	//				(*blockCount)++;

	//				preZ = currentZ;

	//			}
	//		} // z out
	//	}

	//	preZ = 0;
	//	currentZ = 0;
	//}


}

void AWorldMap::CreateTerrain() {
	FVector CurrentSlice = MapLocation;
	size_t cnt{};

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {

			CreateTerrainSlice(cnt);
			cnt += 4;
		}
	}
}

void AWorldMap::CreateUnderground(APartial* NearPartials) {
	FVector tempVector = NearPartials->GetActorLocation();


	FVector Normal = { float(int(tempVector.X) % 10000),
					float(int(tempVector.Y) % 10000),
					tempVector.Z };


	int sliceIndex = (FMath::FloorToInt((Normal.X / 1000)) * 60) +
		(FMath::FloorToInt(Normal.Y / 1000) * 6) +
		(FMath::FloorToInt(Normal.Z / 1000));

	if (sliceIndex < 0 || sliceIndex >= 600)
		return;

	if (UndergroundSlice[sliceIndex]->bAllocated == false) {
		UndergroundSlice[sliceIndex]->bAllocated = true;

		float density{};

		int* blockCount = nullptr;

		APartial* CurrentPartial = UndergroundSlice[sliceIndex];
		FVector Where = CurrentPartial->GetActorLocation();
		ABasicBlock* CurrentBlock = nullptr;
		TArray<BlockData>* CurrentTable = nullptr;


		int _seedX = FMath::FloorToInt(tempVector.X / 100.f);
			
		for (int x = 0; x < 10; x++) {
			int _seedY = FMath::FloorToInt(tempVector.Y / 100.f);

			for (int y = 0; y < 10; y++) {
				int _seedZ = FMath::FloorToInt(tempVector.Z / 100.f);

				for (int z = 10 - 1; z >= 0; z--) {
						
						
					float xnoise = FMath::Abs(MyPerlinNoise(_seedZ, _seedY, UOCATAVES, UNOISESCALE));
					float ynoise = FMath::Abs(MyPerlinNoise(_seedX, _seedY, UOCATAVES, UNOISESCALE));
					float znoise = FMath::Abs(MyPerlinNoise(_seedX, _seedZ, UOCATAVES, UNOISESCALE));

					density = xnoise + ynoise + znoise;

					if (density > UTHRESHOLD) {

						if ((Where.Z + z * 100.f) == (5900.f)) {
							CurrentBlock = CurrentPartial->Grass;
							blockCount = &(CurrentPartial->GrassCount);
							CurrentTable = &(CurrentPartial->GrassTable);
						}
						else {

							// Ore Checker
							float coalProb = 0.02f;
							if (ProbDistributor(coalProb) || coalNum) {
								if(coalNum == 0)
									coalNum = 3;

								CurrentBlock = CurrentPartial->Coal;
								blockCount = &(CurrentPartial->CoalCount);
								CurrentTable = &(CurrentPartial->CoalTable);
								coalNum--;
							}

							else {
								CurrentBlock = CurrentPartial->Rock;
								blockCount = &(CurrentPartial->RockCount);
								CurrentTable = &(CurrentPartial->RockTable);
							}
						}

						FVector Local{ Where.X + x * 100.f, Where.Y + y * 100.f, Where.Z + z * 100.f };
						FString StrTransform = Local.ToString();
						auto BlockTransform = FTransform(Local);

						CurrentBlock->CreateInstance(BlockTransform);

						auto NewBlockData{ BlockData(*blockCount, StrTransform) };
						CurrentTable->Add(NewBlockData);
						(*blockCount)++;
							
					}


					_seedZ++;
				}
				_seedY++;
			}
			_seedX++;
		}
	}
} 

void AWorldMap::CreateTerrainSlice(int pivotIndex) {
	APartial* CurrentPartial = SurfaceSlice[pivotIndex];
	ABasicBlock* CurrentBlock = nullptr;
	TArray<BlockData>* CurrentTable = nullptr;
	FVector Where = CurrentPartial->GetActorLocation();

	float density{};
	int sliceIndex{};
	int preZ{};
	int currentZ{};
	int* blockCount = nullptr;

	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			for (int z = 60 - 1; z >= 0; z--) {
				sliceIndex = pivotIndex + z / 10;
				CurrentPartial = SurfaceSlice[sliceIndex];

				float xnoise = MyPerlinNoise(Where.Z / 100.f + z, Where.Y / 100.f + y, SOCTAVES, SNOISESCALE);
				float ynoise = MyPerlinNoise(Where.X / 100.f + x, Where.Y / 100.f + y, SOCTAVES, SNOISESCALE);
				float znoise = MyPerlinNoise(Where.X / 100.f + x, Where.Z / 100.f + z, SOCTAVES, SNOISESCALE);
				density = xnoise + ynoise + znoise + z;
				
				float val = MyPerlinNoise(x, y, SOCTAVES, SNOISESCALE);
				
				if (val < STHRESHOLD) {
					// Do something..
				}

				// TEST
				/*float xnoise = FMath::PerlinNoise2D(FVector2D{ (Where.Z / 100.f + z) / 20.f, (Where.Y / 100.f + y) / 20.f}) * 12.f;
				float ynoise = FMath::PerlinNoise2D(FVector2D{ (Where.X / 100.f + x) / 20.f, (Where.Y / 100.f + y) / 20.f}) * 12.f;
				float znoise = FMath::PerlinNoise2D(FVector2D{ (Where.X / 100.f + x) / 20.f, (Where.Z / 100.f + z) / 20.f}) * 12.f;
				*/

				if (density < STHRESHOLD) {
					FVector Local{ Where.X + x * 100.f, Where.Y + y * 100.f, Where.Z + z * 100.f };
					currentZ = z * 100;
					

					if (preZ - currentZ != 100) {
						CurrentBlock = CurrentPartial->Grass;
						blockCount = &(CurrentPartial->GrassCount);
						CurrentTable = &(CurrentPartial->GrassTable);

						if (x > 0 && x < 9 &&
							y > 0 && y < 9) {

							float prob = float(z) * 0.003f;

							/*if (ProbDistributor(prob)) {
								CreateTree(Local);
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

bool AWorldMap::ProbDistributor(float prob) {
	std::random_device seed;
	std::default_random_engine eng(seed());
	std::bernoulli_distribution dist(prob);

	return dist(eng);
}

void AWorldMap::CreateTree(FVector Vec) {
	FVector PlantVector{Vec.X, Vec.Y, Vec.Z + 100.f};
	

	CreateLog(PlantVector);
	CreateLeaf(PlantVector);
}

APartial* AWorldMap::GetPartial(FVector Vec) {
	APartial* Output = nullptr;
	FVector Normal = {float(int(Vec.X) % 10000),
					float(int(Vec.Y) % 10000),
					Vec.Z };
	

	// Underground
	if (Vec.Z < 6000.f) {
		int sliceIndex = (FMath::FloorToInt((Normal.X / 1000)) * 60) +
			(FMath::FloorToInt(Normal.Y / 1000) * 6) +
			(FMath::FloorToInt(Normal.Z / 1000));

		if (sliceIndex < 0 || sliceIndex >= 600)
			Output = nullptr;
		else
			Output = UndergroundSlice[sliceIndex];
	}

	// Surface
	else {
		int sliceIndex = (FMath::FloorToInt((Normal.X / 1000)) * 40) +
			(FMath::FloorToInt(Normal.Y / 1000) * 4) +
			(FMath::FloorToInt(Normal.Z / 1000) - 6);

		if (sliceIndex < 0 || sliceIndex >= 400)
			Output = nullptr;
		else
			Output = SurfaceSlice[sliceIndex];

	}

	return Output;
}

void AWorldMap::RemoveCollisionMesh() {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("DESTROY"));
	CollisionMesh->DestroyComponent();
}

void AWorldMap::RunCreaterTask(AWorldMap* aMap) {
	(new FAutoDeleteAsyncTask<MapCreateTask>(aMap))->StartBackgroundTask();
}

void AWorldMap::RunCreaterTaskMain(AWorldMap* aMap) {
	MapCreateTask* Task = new MapCreateTask(aMap);
	
	Task->DoWorkMain();
	delete Task;
}

void AWorldMap::Clear() {
	for (auto iter : SurfaceSlice) {
		//iter->Clean();
		iter->ConditionalBeginDestroy();
	}
	for (auto iter : UndergroundSlice) {
		//iter->Clean();
		iter->ConditionalBeginDestroy();
	}
}

void AWorldMap::CreateLog(FVector Vec) {
	FVector TreeVector = Vec;

	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(TreeVector);
		auto CurrentBlock = CurrentPartial->Tree;
		auto CurrentTable = &(CurrentPartial->TreeTable);
		int* blockCount = &(CurrentPartial->TreeCount);

		FString StrTransform = TreeVector.ToString();
		auto BlockTransform = FTransform(TreeVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData(*blockCount, StrTransform) };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		TreeVector.Set(TreeVector.X, TreeVector.Y, TreeVector.Z + 100.f);
	}
}

void AWorldMap::CreateLeaf(FVector Vec) {
	FVector LeafPivot = { Vec.X, Vec.Y, Vec.Z + 300.f };

	/* left side */
	FVector LeafVector = { LeafPivot.X - 100.f, LeafPivot.Y - 100.f, LeafPivot.Z};
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);

		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);
		
		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}

	LeafVector.Set(LeafPivot.X, LeafPivot.Y - 100.f, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}

	LeafVector.Set(LeafPivot.X + 100.f, LeafPivot.Y - 100.f, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}
	/****/
	/* middle */
	LeafVector.Set(LeafPivot.X - 100.f, LeafPivot.Y, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}

	LeafVector.Set(LeafPivot.X + 100.f, LeafPivot.Y, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}
	LeafVector.Set(LeafPivot.X, LeafPivot.Y, LeafPivot.Z + 200.f);
	FString StrTransform = LeafVector.ToString();
	auto BlockTransform = FTransform(LeafVector);


	auto CurrentPartial = GetPartial(LeafVector);
	auto CurrentBlock = CurrentPartial->Leaf;
	auto CurrentTable = &(CurrentPartial->LeafTable);
	int* blockCount = &(CurrentPartial->LeafCount);
	CurrentBlock->CreateInstance(BlockTransform);

	auto NewBlockData{ BlockData{*blockCount, StrTransform} };
	CurrentTable->Add(NewBlockData);
	(*blockCount)++;
	/****/
	/* Right side */
	LeafVector.Set(LeafPivot.X -100.f, LeafPivot.Y + 100.f, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}
	LeafVector.Set(LeafPivot.X, LeafPivot.Y + 100.f, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}
	LeafVector.Set(LeafPivot.X + 100.f, LeafPivot.Y + 100.f, LeafPivot.Z);
	for (int i = 0; i < 3; i++) {
		auto CurrentPartial = GetPartial(LeafVector);
		auto CurrentBlock = CurrentPartial->Leaf;
		auto CurrentTable = &(CurrentPartial->LeafTable);
		int* blockCount = &(CurrentPartial->LeafCount);
		FString StrTransform = LeafVector.ToString();
		auto BlockTransform = FTransform(LeafVector);

		CurrentBlock->CreateInstance(BlockTransform);

		auto NewBlockData{ BlockData{*blockCount, StrTransform} };
		CurrentTable->Add(NewBlockData);
		(*blockCount)++;

		LeafVector.Z += 100.f;
	}
	/****/
}


MapCreateTask::MapCreateTask(AWorldMap* aMap) {
	if (aMap != nullptr) {
		OutputMap = aMap;
	}
}

MapCreateTask::~MapCreateTask() {
}

void MapCreateTask::DoWork() {
	OutputMap->CreateTerrain();
}

void MapCreateTask::DoWorkMain() {
	DoWork();
}

