// Fill out your copyright notice in the Description page of Project Settings.

#include "SandBoxState.h"
#include "WorldCreater.h"
#include "avatar/Avatar.h"
#include "system/AvatarController.h"

ASandBoxState::ASandBoxState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Data/BaseData.BaseData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_EquipmentDB(TEXT("/Game/Data/EquipmentData.EquipmentData"));
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_BlockDB(TEXT("/Game/Data/BlockData.BlockData"));
	BaseDB = BP_ItemDB.Object;
	EquipmentDB = BP_EquipmentDB.Object;
	BlockDB = BP_BlockDB.Object;
}

void ASandBoxState::BeginPlay() {
	Super::BeginPlay();

	auto WorldCreaterClass = AWorldCreater::StaticClass();
	
	WorldCreater = GetWorld()->SpawnActor<AWorldCreater>(AWorldCreater::StaticClass(),
		FVector(100.f, 100.f, 100.f), FRotator::ZeroRotator);
}

UDataTable* ASandBoxState::GetBaseDB() const {
	return BaseDB;
}

UDataTable* ASandBoxState::GetBlockDB() const {
	return BlockDB;
}

UDataTable* ASandBoxState::GetEquipmentDB() const {
	return EquipmentDB;
}

TArray<BlockData>* ASandBoxState::GetBlockTable(FName ItemID) {
	int id = FCString::Atoi(*ItemID.ToString());

	switch (id) {
	case 1:
		return &DirtTable;
	case 2:
		return &GrassTable;
	case 3:
		return &TreeTable;
	case 5:
		return &RockTable;
	default:
		break;
	}

	return nullptr;
}

bool ASandBoxState::UseBlockItem(ACharacter* Player, FName ItemID, FString BlockName) {
	FString BP_GrassPath = "/Game/Blueprints/BP_" + BlockName + "." + "BP_" + BlockName + "_C";
	UClass* BP_Block = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));

	TArray<BlockData>* BlockTable = GetBlockTable(ItemID);

	if (!IsValid(BP_Block))
		return false;

	auto IPlayer = Cast<AAvatar>(Player);
	auto IController = Cast<AAvatarController>(Player->GetController());
	auto CameraComponent = IPlayer->GetCameraComponent();

	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = (CameraComponent->GetForwardVector() * IPlayer->interactRange) + Start;

	FHitResult CollisionResult;
	FCollisionQueryParams Params(NAME_None, false, IPlayer);
	bool bResult = GetWorld()->SweepSingleByChannel(
		CollisionResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere((1, 1, 1)),
		Params);

	if (bResult) {
		FVector TargetLocation;
		FVector DeployLocation;
		FVector Diff;
		auto HitLocation = CollisionResult.Location;

		TargetLocation = IPlayer->GetBlockLocation(HitLocation);
		DeployLocation = TargetLocation;
		Diff = TargetLocation - HitLocation;

		if (Diff.X >= 1.f)
			DeployLocation.X -= 100.f;
		else if (Diff.X <= -100.f)
			DeployLocation.X += 100.f;

		if (Diff.Y >= 1.f)
			DeployLocation.Y -= 100.f;
		else if (Diff.Y <= -100.f)
			DeployLocation.Y += 100.f;

		if (Diff.Z >= 1.f)
			DeployLocation.Z -= 100.f;
		else if (Diff.Z <= -100.f)
			DeployLocation.Z += 100.f;

		FVector CharaLocation = IPlayer->GetCapsuleComponent()->GetComponentLocation();
		CharaLocation.X = FMath::RoundToInt(CharaLocation.X / 100.f) * 100;
		CharaLocation.Y = FMath::RoundToInt(CharaLocation.Y / 100.f) * 100;
		CharaLocation.Z = int(CharaLocation.Z) / 100 * 100;

		FVector MiddleLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 100.f);
		FVector TopLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 200.f);

		// Check collision between player and block will be deployed.
		if (CharaLocation.Equals(DeployLocation) ||
			MiddleLocation.Equals(DeployLocation) ||
			TopLocation.Equals(DeployLocation)) {
			return false;
		}

		// Build the block.
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
				FString::Printf(TEXT("DeployLocation : %s"), *DeployLocation.ToString()));

			//auto CurrentState = Cast<ASandBoxState>(World->GetGameState());
			auto BlockTransform = FTransform(DeployLocation);
			ABasicBlock* UsingBlock = nullptr;
			FString StrTransform = DeployLocation.ToString();

			// Find block actor.
			if (BlockName.Equals("Dirt"))
				UsingBlock = WorldCreater->Dirt;
			else if(BlockName.Equals("Grass"))
				UsingBlock = WorldCreater->Grass;
			else if (BlockName.Equals("Tree"))
				UsingBlock = WorldCreater->Tree;
			else if (BlockName.Equals("Rock"))
				UsingBlock = WorldCreater->Rock;

			// Validation check of UsingBlock.
			if (UsingBlock) {
				UsingBlock->CreateInstance(BlockTransform);
				int index = UsingBlock->MeshInstances->GetNumRenderInstances() - 1;

				auto BlockData{ TPair<int, FString>(index, StrTransform) };
				BlockTable->Add(BlockData);

				return true;
			}

			// Can't found the correct block name.
			return false;
		}
	}

	return false;
}
