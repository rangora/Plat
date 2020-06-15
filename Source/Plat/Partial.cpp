// Fill out your copyright notice in the Description page of Project Settings.

#include "Partial.h"

APartial::APartial() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TEMP"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TransMesh(TEXT("/Game/resources/blocks/PartialMesh.PartialMesh"));

	if (TransMesh.Succeeded())
		CollisionMesh->SetStaticMesh(TransMesh.Object);

	CollisionMesh->SetRelativeScale3D(FVector(10.f, 10.f, 10.f));
	CollisionMesh->SetCollisionProfileName(TEXT("BlockQuery"));
}

APartial::~APartial() {
}

void APartial::BeginPlay() {
	Super::BeginPlay();

	FString BP_DirtPath   = "/Game/Blueprints/BP_Dirt.BP_Dirt_C";
	FString BP_GrassPath  = "/Game/Blueprints/BP_Grass.BP_Grass_C";
	FString BP_RockPath   = "/Game/Blueprints/BP_Rock.BP_Rock_C";
	FString BP_TreePath   = "/Game/Blueprints/BP_Tree.BP_Tree_C";
	FString BP_LumberPath = "/Game/Blueprints/BP_Lumber.BP_Lumber_C";
	FString BP_LeafPath = "/Game/Blueprints/BP_Leaf.BP_Leaf_C";
	FString BP_CoalPath = "/Game/Blueprints/BP_Coal.BP_Coal_C";

	UClass* BP_Dirt = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_DirtPath));
	UClass* BP_Grass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));
	UClass* BP_Rock = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_RockPath));
	UClass* BP_Tree = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_TreePath));
	UClass* BP_Lumber = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_LumberPath));
	UClass* BP_Leaf = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_LeafPath));
	UClass* BP_Coal = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_CoalPath));

	Dirt    = GetWorld()->SpawnActor<ABasicBlock>(BP_Dirt, FVector::ZeroVector, FRotator::ZeroRotator);
	Grass   = GetWorld()->SpawnActor<ABasicBlock>(BP_Grass, FVector::ZeroVector, FRotator::ZeroRotator);
	Rock    = GetWorld()->SpawnActor<ABasicBlock>(BP_Rock, FVector::ZeroVector, FRotator::ZeroRotator);
	Tree    = GetWorld()->SpawnActor<ABasicBlock>(BP_Tree, FVector::ZeroVector, FRotator::ZeroRotator);
	Lumber  = GetWorld()->SpawnActor<ABasicBlock>(BP_Lumber, FVector::ZeroVector, FRotator::ZeroRotator);
	Leaf  = GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf, FVector::ZeroVector, FRotator::ZeroRotator);
	Coal  = GetWorld()->SpawnActor<ABasicBlock>(BP_Coal, FVector::ZeroVector, FRotator::ZeroRotator);
}

TArray<BlockData>* APartial::GetBlockTable(FName BlockID) {
	int id = FCString::Atoi(*BlockID.ToString());
	TArray<BlockData>* TargetTable = nullptr;

	switch (id) {
	case DIRT:
		TargetTable = &DirtTable;
		break;

	case GRASS:
		TargetTable = &GrassTable;
		break;

	case ROCK:
		TargetTable = &RockTable;
		break;

	case TREE:
		TargetTable = &TreeTable;
		break;

	case LUMBER:
		TargetTable = &LumberTable;
		break;

	case LEAF:
		TargetTable = &LeafTable;
		break;

	case COAL:
		TargetTable = &CoalTable;
		break;

	default:
		TargetTable = nullptr;
		break;
	}

	return TargetTable;
}

ABasicBlock* APartial::GetBlock(FName BlockID) {
	int id = FCString::Atoi(*BlockID.ToString());
	ABasicBlock* TargetBlock = nullptr;

	switch (id) {
	case DIRT:
		TargetBlock = Dirt;
		break;

	case GRASS:
		TargetBlock = Grass;
		break;

	case ROCK:
		TargetBlock = Rock;
		break;

	case TREE:
		TargetBlock = Tree;
		break;

	case LUMBER:
		TargetBlock = Lumber;
		break;

	case LEAF:
		TargetBlock = Leaf;
		break;

	case COAL:
		TargetBlock = Coal;
		break;

	default:
		break;
	}

	return TargetBlock;
}

int* APartial::GetBlockCount(FName BlockID) {
	int id = FCString::Atoi(*BlockID.ToString());
	int* TargetCount = nullptr;

	switch (id) {
	case DIRT:
		TargetCount = &DirtCount;
		break;

	case GRASS:
		TargetCount = &GrassCount;
		break;

	case ROCK:
		TargetCount = &RockCount;
		break;

	case TREE:
		TargetCount = &TreeCount;
		break;

	case LUMBER:
		TargetCount = &LumberCount;
		break;

	case LEAF:
		TargetCount = &LeafCount;
		break;

	case COAL:
		TargetCount = &CoalCount;
		break;

	default:
		break;
	}

	return TargetCount;
}

void APartial::Clean() {
	if (Dirt->IsValidLowLevel()) {
		Dirt->Destroy();
	}
	if (Grass->IsValidLowLevel()) {
		Grass->Destroy();
	}
	if (Rock->IsValidLowLevel()) {
		Rock->Destroy();
	}
	if (Tree->IsValidLowLevel()) {
		Tree->Destroy();
	}
	if (Lumber->IsValidLowLevel()) {
		Lumber->Destroy();
	}
	if (Leaf->IsValidLowLevel()) {
		Leaf->Destroy();
	}
	if (Coal->IsValidLowLevel()) {
		Coal->Destroy();
	}
}
