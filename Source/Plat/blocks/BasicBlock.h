// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/Block.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "item/CBlockData.h"
#include "BasicBlock.generated.h"

/**
 *
 */

class ACrackingMesh;

UCLASS()
class PLAT_API ABasicBlock : public ABlock {
	GENERATED_BODY()

public:
	ABasicBlock();
	~ABasicBlock();

	void DropItem(FVector DropLocation);
	void Reset();

	const FName GetItemID() const;

	void CreateInstance(FTransform& BlockTransform);

	void Breaking();

public:
	/* If is equal with 'Match' int weapon class, this block get huge damage. */
	/* If is equal with 'Match' int weapon class, this block get huge damMaterialsage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* MeshInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxHP;

	float currentHP;

	/* Mesh instance value. */
	size_t instanceIndex;
	FVector BlockLocation;
	ACrackingMesh* CrackingEffect = nullptr;
};
