// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/Block.h"
#include "blocks/BasicBlockComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "item/CBlockData.h"
#include "BasicBlock.generated.h"

/**
 *
 */

UCLASS()
class PLAT_API ABasicBlock : public ABlock {
	GENERATED_BODY()

public:
	ABasicBlock();

	void DropItem(FVector DropLocation);
	void Restore();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	const FName GetItemID() const;

	void CreateInstance(FTransform& BlockTransform);

public:
	/* If is equal with 'Match' int weapon class, this block get huge damage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* MeshInstances;

	float maxHP;

protected:
	/* Unique value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(VisibleAnywhere)
		class UBasicBlockComponent* BlockStat;
};
