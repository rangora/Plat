// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/Block.h"
#include "blocks/BasicBlockComponent.h"
#include "BasicBlock.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API ABasicBlock : public ABlock {
	GENERATED_BODY()
	
public:
	ABasicBlock();

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool UseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlockMesh;

	UPROPERTY(VisibleAnywhere)
		class UBasicBlockComponent* BlockStat;
};
