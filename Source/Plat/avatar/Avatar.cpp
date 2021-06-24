// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "WorldCreater.h"
#include "Partial.h"
#include "item/CEquipmentData.h"
#include "system/AvatarController.h"
#include "DrawDebugHelpers.h"

AAvatar::AAvatar() {
	PrimaryActorTick.bCanEverTick = true;
	isAttacking = false;
	AttackRange = 200.;
	AttackRadius = 50.;
	attackPower = BASEATTACKPOWER;
	healthValue = 1.0;

	// Create SpringArmComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// Create CameraComponents
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	CameraComponent->SetupAttachment(SpringArm);

	// Common view setting.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	CameraComponent->bUsePawnControlRotation = false;	// 카메라 회전 시, 캐릭터 회전 여부.
	SpringArm->bUsePawnControlRotation = true;			// 가능한 회전 제어를 사용하는지 여부.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Create Actor Character
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Avatar(
		TEXT("/Game/resources/character/steve/steve.steve"));

	if (SK_Avatar.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_Avatar.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AVATAR_ANIM(
		TEXT("/Game/resources/character/steve/SteveAnimInstance.SteveAnimInstance_C"));

	if (AVATAR_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(AVATAR_ANIM.Class);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 87.0f), FRotator(0.0f, 0.0f, 0.0f)); // Set pivot
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	// Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAvatar::PickupItem);

	// create the collection sphere for auto pick up system
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(350.f);

	// Map trigger
	CreateUndergroundSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LoadUndergroundSphere"));
	CreateUndergroundSphere->SetupAttachment(RootComponent);
	CreateUndergroundSphere->SetSphereRadius(1500.f);

	CreateMapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LoadMapSphere"));
	CreateMapSphere->SetupAttachment(RootComponent);
	CreateMapSphere->SetRelativeLocation(FVector{0.f, 0.f, 3300.f});
	CreateMapSphere->SetSphereRadius(2500.f);

	// jump
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// Weapon
	Weapon = nullptr;

	// view
	CurrentView = ViewState::FIRSTPERSON;
	ViewChange();
}

void AAvatar::UpDown(float newAxisValue) {
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, newAxisValue);
}

void AAvatar::LeftRight(float newAxisValue) {
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, newAxisValue);
}

void AAvatar::LookUp(float newAxisValue) {
	AddControllerPitchInput(newAxisValue);
}

void AAvatar::Turn(float newAxisValue) {
	AddControllerYawInput(newAxisValue);
}

void AAvatar::NoCollision() {
	auto direction = CameraComponent->GetForwardVector();

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetMesh()->SetCollisionProfileName("NoCollision");
}

void AAvatar::AttackTarget() {
	if (TargetBlock != nullptr) {
		float amountPower{};

		amountPower = GetAttackDamage();

		TargetBlock->currentHP -= amountPower;

		if (TargetBlock->currentHP <= 0) {
			// Destroy the block.
			if (DestroyTargetBlock()) {
				EndHit();
			};
		}
		else {
			TargetBlock->Breaking();
		}
		
	}
	else {
		EndHit();
	}
}

float AAvatar::GetAttackDamage() {
	float total{};

	if (Weapon != nullptr) {
		if (Weapon->Match == TargetBlock->Match) {
			total += Weapon->adDamage;
		}
		else {
			total += Weapon->damage;
		}
	}
	else {
		total = BASEATTACKPOWER;
	}

	return total;
}

void AAvatar::SetTargetBlock(AActor* HitBlock, FVector BlockVector) {
	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	auto TempBlock = Cast<ABasicBlock>(HitBlock);
	BlockData* TargetBlockData = nullptr;

	int mapIndex = CurrentState->WorldCreater->GetMapIndex(BlockVector);

	if (mapIndex < 0)
		return;

	CurrentPartial = CurrentState->WorldCreater->GetAPartialCurrentMap(BlockVector, mapIndex);

	if (CurrentPartial != nullptr) {

		TargetBlock = CurrentPartial->GetBlock(TempBlock->ItemID);
		CurrentTable = CurrentPartial->GetBlockTable(TempBlock->ItemID);

		if (TargetBlock != nullptr && IsValid(TargetBlock)) {

			if (CurrentTable != nullptr) {

				for (auto iter : *CurrentTable) {
					if (iter.Value.Equals(BlockVector.ToString())) {
						TargetBlockData = &iter;
						break;
					}

				}

				if (TargetBlockData != nullptr) {
					TargetBlock->BlockLocation = BlockVector;
					TargetBlock->instanceIndex = TargetBlockData->Key; // 위험.
				}
			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
	//	FString::Printf(TEXT("BlockVector:%s"), *BlockVector.ToString()));
}

AActor* AAvatar::GetForwardBlock(FVector* HitLocation) {
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = (CameraComponent->GetForwardVector() * AttackRange) + Start;

	FHitResult HitResult;

	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(1.f),
		Params);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);

	if (bResult) {
		if (HitLocation) {
			*HitLocation = HitResult.Location;
		}
		return HitResult.GetActor();
	}
	return nullptr;
}

bool AAvatar::DestroyTargetBlock() {
	int targetIndex = TargetBlock->instanceIndex;
	
	if (targetIndex >= 0) {
		// Remove the block instance.
		if (TargetBlock->MeshInstances->RemoveInstance(targetIndex)) {
			// Remove the block data from BlockTable and refresh the key values.
			for (int i = targetIndex + 1; i < CurrentTable->Num(); i++) {
				if (CurrentTable->IsValidIndex(i)) 
					(*CurrentTable)[i].Key -= 1;
			}

			if (CurrentTable->IsValidIndex(targetIndex))
				CurrentTable->RemoveAt(targetIndex);
			
			auto CurrentCount = CurrentPartial->GetBlockCount(TargetBlock->ItemID);
			(*CurrentCount)--;

			// Spawn pick up item.
			TargetBlock->DropItem(FVector(TargetBlock->BlockLocation.X + 50.f,
				TargetBlock->BlockLocation.Y + 50.f,
				TargetBlock->BlockLocation.Z + 50.f));

			// Reset parameters.
			TargetBlock->Reset();

			return true;
		}
	}

	return false;
}

bool AAvatar::IsVectorOverlapped(FVector DeployVector) {
	FVector CharaLocation = GetCapsuleComponent()->GetComponentLocation();
	CharaLocation.X = FMath::RoundToInt(CharaLocation.X / 100.f) * 100;
	CharaLocation.Y = FMath::RoundToInt(CharaLocation.Y / 100.f) * 100;
	CharaLocation.Z = int(CharaLocation.Z) / 100 * 100;
	
	FVector MiddleLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 100.f);
	FVector TopLocation = FVector(CharaLocation.X, CharaLocation.Y, CharaLocation.Z + 200.f);

	// Check collision between player and block will be deployed.
	if (CharaLocation.Equals(DeployVector) ||
		MiddleLocation.Equals(DeployVector) ||
		TopLocation.Equals(DeployVector)) {
		return true;
	}

	return false;
}

void AAvatar::OnHit() {
	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	
	ResetTargetBlock();

	AttackAnim();

	AActor* HitBlock = GetForwardBlock(&HitLocation);
	
	
	if (HitBlock != nullptr) {
		FVector BlockVector =  GetBlockLocation(HitLocation);

		SetTargetBlock(HitBlock, BlockVector);
		


		GetWorldTimerManager().SetTimer(AttackAnimTimer, this, &AAvatar::AttackAnim, 0.3f, true);
		GetWorldTimerManager().SetTimer(BreakBlockTimer, this, &AAvatar::AttackTarget, 0.3f, true);

		bIsAtackking = true;
	}
}

void AAvatar::EndHit() {
	GetWorldTimerManager().ClearTimer(AttackAnimTimer);
	GetWorldTimerManager().ClearTimer(BreakBlockTimer);

	bIsAtackking = false;
	

	if (TargetBlock != nullptr) {
		TargetBlock->Reset();
	}

	ResetTargetBlock();
}

void AAvatar::ResetTargetBlock() {
	TargetBlock = nullptr;
	CurrentPartial = nullptr;
	CurrentTable = nullptr;
}

void AAvatar::AttackAnim() {
	ABAnim->PlayAttackMontage();
}

void AAvatar::LoadMaps() {
	FVector PlayerVec = GetActorLocation();

	auto MapNear = GetNearMap();
	Creater->AutoMapLoader(PlayerVec, MapNear);
}

void AAvatar::CreateUnderground() {
	auto UndergroundNear = GetNearUndergroundPartials();
	Creater->AutoUndergroundCreater(UndergroundNear);
}

TArray<AWorldMap*> AAvatar::GetNearMap() {
	TArray<AActor*> CollectActors;
	TArray<AWorldMap*> Near;
	AWorldMap* PieceOfMap = nullptr;

	CreateMapSphere->GetOverlappingActors(CollectActors);

	for (auto iter : CollectActors) {
		PieceOfMap = Cast<AWorldMap>(iter);

		if (IsValid(PieceOfMap) && !PieceOfMap->IsPendingKill()) {
			Near.Add(PieceOfMap);
		}
	}

	return Near;
}

TArray<APartial*> AAvatar::GetNearUndergroundPartials() {
	TArray<AActor*> CollectActors;
	TArray<APartial*> Near;
	APartial* Piece = nullptr;


	CreateUndergroundSphere->GetOverlappingActors(CollectActors);

	for (int32 Collected = 0; Collected < CollectActors.Num(); ++Collected) {
		Piece = Cast<APartial>(CollectActors[Collected]);
		
		if (Piece) {
			if (Piece->GetActorLocation().Z < 6000.f) {
				Near.Add(Piece);
			}
		}
	}
	return Near;
}

void AAvatar::ShowCurrentVector() {
	FVector Current = GetActorLocation();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
		FString::Printf(TEXT("Current:%s"), *Current.ToString()));
}

void AAvatar::UseItem() {
	auto IController = Cast<AAvatarController>(GetController());
	int index = IController->ScreenUIWidget->GetUsingIndex();

	// Check quick slot valiable.
	if (index < 0) {
		return;
	}
	IController->ScreenUIWidget->QuickSlots[index]->UseItem();
}

void AAvatar::CollectAutoPickups() {
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	AAvatarController* IController = Cast<AAvatarController>(GetController());

	// For each collected Actor
	for (int32 Collected = 0; Collected < CollectedActors.Num(); ++Collected) {
		// Cast the actor to AAutoPickup
		AAutoPickup* const TestPickup = Cast<AAutoPickup>(CollectedActors[Collected]);
		// If the cast is successful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill())
			TestPickup->Collect(IController);
	}
}

bool AAvatar::DeployBlock(FName ItemID, FString BlockName) {
	FString BP_GrassPath = "/Game/Blueprints/BP_" + BlockName + "." + "BP_" + BlockName + "_C";
	UClass* BP_Block = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));

	if (!IsValid(BP_Block))
		return false;

	AActor* HitBlock = GetForwardBlock(&HitLocation);

	if (IsValid(HitBlock)) {
		FVector DeployLocation = GetDeployLocation(HitLocation);

		// Check the deployed vector is overlapped with character.
		if (IsVectorOverlapped(DeployLocation)) {
			return false;
		}

		// Build the block.
		else {
			FTransform BlockTransform = FTransform(DeployLocation);
			FString StrTransform = DeployLocation.ToString();
			auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());
			
			int mapIndex = CurrentState->WorldCreater->GetMapIndex(DeployLocation);
			
			if (mapIndex < 0)
				return false;

			CurrentPartial = CurrentState->WorldCreater->GetAPartialCurrentMap(DeployLocation, mapIndex);
			CurrentTable = CurrentPartial->GetBlockTable(ItemID);

			auto tt=CurrentPartial->GetActorLocation();

			if (CurrentPartial != nullptr && CurrentTable != nullptr) {
				auto UsingBlock = CurrentPartial->GetBlock(ItemID);

				if (UsingBlock) {
					UsingBlock->CreateInstance(BlockTransform);
					int index = UsingBlock->MeshInstances->GetNumRenderInstances() - 1;

					auto BlockData{ TPair<int, FString>(index, StrTransform) };
					CurrentTable->Add(BlockData);
					ResetTargetBlock();
					return true;
				}
			}
		}
	}
	return false;
}

FVector AAvatar::GetBlockLocation(FVector HitLocation) {
	FVector BlockLocation;

	if (int((HitLocation.X + 2.f) / 100.f) > int(HitLocation.X / 100.f))
		BlockLocation.X = FMath::FloorToFloat((HitLocation.X + 2.f) / 100.f) * 100.f;
	else
		BlockLocation.X = FMath::FloorToFloat((HitLocation.X - 2.f) / 100.f) * 100.f;

	if (int((HitLocation.Y + 2.f) / 100.f) > int(HitLocation.Y / 100.f))
		BlockLocation.Y = FMath::FloorToFloat((HitLocation.Y + 2.f) / 100.f) * 100.f;
	else
		BlockLocation.Y = FMath::FloorToFloat((HitLocation.Y - 2.f) / 100.f) * 100.f;

	if (int((HitLocation.Z + 2.f) / 100.f) > int(HitLocation.Z / 100.f))
		BlockLocation.Z = FMath::FloorToFloat((HitLocation.Z + 2.f) / 100.f) * 100.f;
	else
		BlockLocation.Z = FMath::FloorToFloat((HitLocation.Z - 2.f) / 100.f) * 100.f;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//	FString::Printf(TEXT("BlockLocation:%s"), *BlockLocation.ToString()));

	return BlockLocation;
}

FVector AAvatar::GetDeployLocation(FVector HitVector) {
	FVector TargetBlockVector = GetBlockLocation(HitVector);
	FVector DeployVector = TargetBlockVector;
	FVector Diff = TargetBlockVector - HitVector;

	if (Diff.X >= 1.f)
		DeployVector.X -= 100.f;
	else if (Diff.X <= -100.f)
		DeployVector.X += 100.f;

	if (Diff.Y >= 1.f)
		DeployVector.Y -= 100.f;
	else if (Diff.Y <= -100.f)
		DeployVector.Y += 100.f;

	if (Diff.Z >= 1.f)
		DeployVector.Z -= 100.f;
	else if (Diff.Z <= -100.f)
		DeployVector.Z += 100.f;

	return DeployVector;
}

void AAvatar::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	isAttacking = false;
}

float AAvatar::GetHealthValue() {
	return healthValue;
}

AEquipment* AAvatar::GetWeapon() {
	return Weapon;
}

UCameraComponent* AAvatar::GetCameraComponent() {
	return CameraComponent;
}

void AAvatar::SetWeapon(AEquipment* NewWeapon, FName ID) {
	auto IState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	FEquipmentData* Equipment = IState->EquipmentDB->FindRow<FEquipmentData>(ID, "");

	if (Equipment != nullptr) {
		Weapon = NewWeapon;
		Weapon->damage = Equipment->Damage;
		Weapon->adDamage = Equipment->AdDamage;
		Weapon->Match = Equipment->Match;
	}
}

void AAvatar::DisarmWeapon() {
	if (Weapon)
		Weapon->Destroy();
	Weapon = nullptr;
	attackPower = BASEATTACKPOWER;
}

void AAvatar::BeginPlay() {
	Super::BeginPlay();

	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());
	Creater = CurrentState->GetWorldCreater();

	GetWorldTimerManager().SetTimer(MapLoadTimer, this, &AAvatar::LoadMaps, 1.0f, true, 1.0f);
	//GetWorldTimerManager().SetTimer(UndergroundCreateTimer, this, &AAvatar::CreateUnderground, 0.8f, true, 0.2f);
}

void AAvatar::ViewChange() {
	switch (CurrentView) {
	
		// FirstView -> ThirdView(Start view)
	case ViewState::FIRSTPERSON: {
		SpringArm->TargetArmLength = 900.0f;
		SpringArm->SetRelativeLocation(FVector::ZeroVector);
		AttackRange += SpringArm->TargetArmLength;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		// Shadow on.
		GetMesh()->bCastDynamicShadow = true;
		GetMesh()->CastShadow = true;

		CurrentView = ViewState::THIRDPERSON;
		break;
	}
	// ThriedView -> FirstView
	case ViewState::THIRDPERSON: {
		SpringArm->TargetArmLength = 0.f;
		SpringArm->SetRelativeLocation(FVector(15.f, 0.f, 72.f));
		AttackRange -= SpringArm->TargetArmLength;

		GetCharacterMovement()->bOrientRotationToMovement = false;		// 방향키 방향으로 캐릭터가 회전함.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;	// 마우스 방향으로 캐릭터도 회전함.

		// Shadow off.
		GetMesh()->bCastDynamicShadow = false;
		GetMesh()->CastShadow = false;

		CurrentView = ViewState::FIRSTPERSON;
		break;
	}
	}
}

void AAvatar::PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AAutoPickup* Item = Cast<AAutoPickup>(OtherActor);

	if (IsValid(Item)) {
		auto IController = Cast<AAvatarController>(GetController());
		if (IController->AddItemToInventory(Item->GetItemID()))
			Item->Destroy();
	}
}

void AAvatar::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CollectAutoPickups();
	//CreateMaps();
}

void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAvatar::Jump);
	PlayerInputComponent->BindAction(TEXT("UseItem"), EInputEvent::IE_Pressed, this, &AAvatar::UseItem);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AAvatar::ViewChange);
	PlayerInputComponent->BindAction(TEXT("NoCollision"), EInputEvent::IE_Pressed, this, &AAvatar::NoCollision);
	PlayerInputComponent->BindAction(TEXT("ShowVec"), EInputEvent::IE_Pressed, this, &AAvatar::ShowCurrentVector);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AAvatar::OnHit);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &AAvatar::EndHit);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AAvatar::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AAvatar::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAvatar::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAvatar::Turn);
}

void AAvatar::PostInitializeComponents() {
	Super::PostInitializeComponents();

	ABAnim = Cast<UAvatarAnimInstance>(GetMesh()->GetAnimInstance());
	ABAnim->OnMontageEnded.AddDynamic(this, &AAvatar::OnAttackMontageEnded);
}