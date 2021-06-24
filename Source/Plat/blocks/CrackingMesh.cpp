// Fill out your copyright notice in the Description page of Project Settings.

#include "CrackingMesh.h"

ACrackingMesh::ACrackingMesh() {
	PrimaryActorTick.bCanEverTick = false;

	Crack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crack"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CrackingMesh(TEXT("/Game/resources/blocks/Cracked.Cracked"));

	if (CrackingMesh.Succeeded())
		Crack->SetStaticMesh(CrackingMesh.Object);

	RootComponent = Crack;
}