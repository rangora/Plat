// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "item/CBlockData.h"
#include "Equipment.generated.h"

UCLASS()
class PLAT_API AEquipment : public AActor {
	GENERATED_BODY()

public:
	AEquipment();

	virtual void PostInitializeComponents() override;
	bool SetWeaponStaticMesh(const FString& ContentPath, const FString& Name);
	FString GetWeaponName();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		FString Name;

public:
	float damage;
	float adDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMatch Match;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
};
