// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBlock.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"
#include "item/AutoPickup.h"

ABasicBlock::ABasicBlock() {
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	BlockStat = CreateDefaultSubobject<UBasicBlockComponent>(TEXT("BlockStat"));
	RootComponent = Cast<USceneComponent>(BlockMesh);

	BlockMesh->SetCollisionProfileName(TEXT("Block"));	

	ItemID = FName("NO_ID");
	Super::Name = "BasicBlock";
}

void ABasicBlock::DropItem() {
	if (Name.Equals("BasicBlock"))
		return;
	else {
		FString BP_ItemPath = "/Game/Blueprints/Auto_" + Name + "." + "Auto_" + Name + "_C";
		UClass* BP_Item = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_ItemPath));

		if (IsValid(BP_Item)) {
			FVector DropLocation = GetActorLocation();
			GetWorld()->SpawnActor<AAutoPickup>(BP_Item,
				DropLocation, FRotator::ZeroRotator);
		}
	}
}

void ABasicBlock::PostInitializeComponents() {
	Super::PostInitializeComponents();

	BlockStat->OnHPIsZero.AddLambda([this]() {
		SetActorEnableCollision(false);
		DropItem();
		Destroy();
		});
}

float ABasicBlock::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float finalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	BlockStat->SetDamage(finalDamage);
	return finalDamage;
}

bool ABasicBlock::UseItem(ACharacter* Player, APlayerController* Contrller, UWorld* World, FString BlockName) {
	FString BP_GrassPath = "/Game/Blueprints/BP_" + BlockName + "." + "BP_" + BlockName + "_C"; 
	UClass* BP_Block = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));
	
	if (!IsValid(BP_Block))
		return false;
	
	auto IPlayer = Cast<AAvatar>(Player);
	auto IController = Cast<AAvatarController>(Contrller);
	auto CameraComponent = IPlayer->GetCameraComponent();

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = (CameraComponent->GetForwardVector() * IPlayer->interactRange) + Start;

	FHitResult CollisionResult;
	FCollisionQueryParams Params(NAME_None, false, IPlayer);
	bool bResult = World->SweepSingleByChannel(
		CollisionResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere((1, 1, 1)),
		Params);

	if (bResult) {
		auto _hitLocation = CollisionResult.Location;
		auto _blockActor = CollisionResult.GetActor();
		//FVector CharaLocation = CameraComponent->GetComponentLocation();
		FVector _targetLocation = _blockActor->GetActorLocation();
		FVector _deployLocation = _targetLocation;
		FVector _diff = _targetLocation - _hitLocation;

		if (_diff.X < -50.f)
			_deployLocation.X += 100;
		else if (_diff.X > 50.f)
			_deployLocation.X -= 100;
		else if (_diff.Y < -50.f)
			_deployLocation.Y += 100;
		else if (_diff.Y > 50.f)
			_deployLocation.Y -= 100;
		else if (_diff.Z < -100.f)
			_deployLocation.Z += 100;
		else if (_diff.Z > 1.f)
			_deployLocation.Z -= 100;

		FVector CharaLocation = IPlayer->GetCapsuleComponent()->GetComponentLocation();
		CharaLocation.X = FMath::RoundToInt(CharaLocation.X / 100.f) * 100;
		CharaLocation.Y = FMath::RoundToInt(CharaLocation.Y / 100.f) * 100;
		CharaLocation.Z = int(CharaLocation.Z) / 100 * 100;

		FVector MiddleLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 100.f);
		FVector TopLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 200.f);

		// Check collision between player and block will be deployed.
		if (CharaLocation.Equals(_deployLocation) ||  
			MiddleLocation.Equals(_deployLocation) ||
				TopLocation.Equals(_deployLocation)) {
			return false;
		}

		// Build the block.
		else {		
			World->SpawnActor<ABasicBlock>(BP_Block, _deployLocation, FRotator::ZeroRotator);
			return true;
		}
	}

	return false;
}
