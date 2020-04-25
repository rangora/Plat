// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "blocks/Block.h"
#include "blocks/BasicBlock.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AvatarEquipment.generated.h"

UCLASS()
class PLAT_API AAvatarEquipment : public AActor {
	GENERATED_BODY()
	
public:	
	AAvatarEquipment();	

	virtual void PostInitializeComponents() override;
	void SetWeaponStaticMesh(const FString& ContentPath, const FString& Name);
	FString GetWeaponName();

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
		FString Name;

public:	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMeshComponent = nullptr;
};
