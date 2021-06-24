// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TreeCreater.generated.h"

UCLASS()
class PLAT_API ATreeCreater : public AActor {
	GENERATED_BODY()

public:
	ATreeCreater();

	virtual void BeginPlay() override;

	void PlantTree();

private:
	bool CheckSpace();
	void CreateLog();
	void CreateLeaf();

public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BaseLog;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	USceneComponent* Root;

private:
	FVector PlantLocation;
	FVector CurrentLocation;
	FString BP_LogPath;
	FString BP_LeafPath;
};
