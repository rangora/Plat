// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Components/ActorComponent.h"
#include "BasicBlockComponent.generated.h"


class AManPickup;

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAT_API UBasicBlockComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UBasicBlockComponent();
	
	void SetDamage(float NewDamage);
	void Restore();

protected:
	virtual void BeginPlay() override;

public:	
	FOnHPIsZeroDelegate OnHPIsZero;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* DropItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float maxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
		float currentHP;
		
};
