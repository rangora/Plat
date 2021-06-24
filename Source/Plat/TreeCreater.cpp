// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeCreater.h"
#include "blocks/BasicBlock.h"

ATreeCreater::ATreeCreater() {
	PrimaryActorTick.bCanEverTick = false;

	BaseLog = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		TreeLog(TEXT("/Game/resources/blocks/Tree.Tree"));

	if (TreeLog.Succeeded())
		BaseLog->SetStaticMesh(TreeLog.Object);

	RootComponent = BaseLog;
	BP_LogPath = "/Game/Blueprints/BP_Tree.BP_Tree_C";
	BP_LeafPath = "/Game/Blueprints/BP_Leaf.BP_Leaf_C";
}

void ATreeCreater::BeginPlay() {
	Super::BeginPlay();
	PlantTree();
}

void ATreeCreater::PlantTree() {
	if (CheckSpace()) {
		CreateLog();
		CreateLeaf();
	}
}

bool ATreeCreater::CheckSpace() {
	return true;
}

void ATreeCreater::CreateLog() {
	CurrentLocation = GetActorLocation();
	UClass* BP_Log = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_LogPath));

	for (int i = 0; i < 3; i++) {
		CurrentLocation.Z += 100.f;
		GetWorld()->SpawnActor<ABasicBlock>(BP_Log,
			CurrentLocation, FRotator::ZeroRotator);
	}
}

void ATreeCreater::CreateLeaf() {
	//CurrentLocation.Z += 100.f;
	UClass* BP_Leaf = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_LeafPath));

	// 1.
	// left
	FVector tempVec = CurrentLocation;
	tempVec.Y -= 200.f;
	tempVec.X -= 100.f;
	for (int i = 0; i < 3; i++) {
		GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf,
			tempVec, FRotator::ZeroRotator);
		tempVec.X += 100.f;
	}

	// top
	tempVec = CurrentLocation;
	tempVec.Y -= 100.f;
	tempVec.X += 200.f;
	for (int i = 0; i < 3; i++) {
		GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf,
			tempVec, FRotator::ZeroRotator);
		tempVec.Y += 100.f;
	}

	// right
	tempVec = CurrentLocation;
	tempVec.Y += 200.f;
	tempVec.X -= 100.f;
	for (int i = 0; i < 3; i++) {
		GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf,
			tempVec, FRotator::ZeroRotator);
		tempVec.X += 100.f;
	}

	// bottom
	tempVec = CurrentLocation;
	tempVec.Y -= 100.f;
	tempVec.X -= 200.f;
	for (int i = 0; i < 3; i++) {
		GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf,
			tempVec, FRotator::ZeroRotator);
		tempVec.Y += 100.f;
	}

	// 2 & 3.
	tempVec = CurrentLocation;
	tempVec.X -= 100.f;
	tempVec.Y -= 100.f;
	for (int i = 0; i < 2; i++) {
		tempVec.Z += 100.f;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				GetWorld()->SpawnActor<ABasicBlock>(BP_Leaf,
					FVector(tempVec.X + 100.f * j,
						tempVec.Y + 100.f * k,
						tempVec.Z), FRotator::ZeroRotator);
			}
		}
	}
}