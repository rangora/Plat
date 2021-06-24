// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "blocks/BasicBlock.h"
#include "GameFramework/Actor.h"
#include "Partial.generated.h"

typedef TPair<int, FString> BlockData;

UCLASS()
class PLAT_API APartial : public AActor {
	GENERATED_BODY()

public:
	APartial();
	~APartial();

	virtual void BeginPlay() override;

	TArray<BlockData>* GetBlockTable(FName BlockID);
	ABasicBlock* GetBlock(FName BlockID);
	int* GetBlockCount(FName BlockID);

	void Clean();

public:
	static size_t DirtAmount;
	static size_t GrassAmount;
	static size_t RockAmount;

	bool bAllocated = false;

	int DirtCount{};
	int GrassCount{};
	int RockCount{};
	int TreeCount{};
	int LumberCount{};
	int LeafCount{};
	int CoalCount{};

	UPROPERTY()
		ABasicBlock* Dirt;
	UPROPERTY()
		ABasicBlock* Grass;
	UPROPERTY()
		ABasicBlock* Rock;
	UPROPERTY()
		ABasicBlock* Tree;
	UPROPERTY()
		ABasicBlock* Lumber;
	UPROPERTY()
		ABasicBlock* Leaf;
	UPROPERTY()
		ABasicBlock* Coal;
	
	TArray<BlockData> DirtTable;
	TArray<BlockData> GrassTable;
	TArray<BlockData> RockTable;
	TArray<BlockData> TreeTable;
	TArray<BlockData> LumberTable;
	TArray<BlockData> LeafTable;
	TArray<BlockData> CoalTable;

	UStaticMeshComponent* CollisionMesh;

private:
	enum {
		DIRT = 1,
		GRASS = 2,
		TREE = 3,
		LUMBER = 4,
		ROCK = 5,
		LEAF = 6,
		COAL = 7
	};

};

size_t APartial::DirtAmount = 0;
size_t APartial::GrassAmount = 0;
size_t APartial::RockAmount = 0;