// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "item/CEquipmentData.h"
#include "DrawDebugHelpers.h"

AAvatar::AAvatar() {
	PrimaryActorTick.bCanEverTick = true;
	isAttacking = false;
	AttackRange = 200.;
	AttackRadius = 50.;
	attackPower = 10.;
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
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Avatar"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAvatar::PickupItem);

	// create the collection sphere for auto pick up system
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(350.f);

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

void AAvatar::AttackTarget() {
	FVector HitLocation;
	AActor* HitActor = GetForwardBlock(&HitLocation);

	// Validation check.
	if (IsValid(HitActor)) {
		// Get location of a hit block.	
		FVector BlockLocation = GetBlockLocation(HitLocation);

		// Destroy the block.
		if (DestroyHitBlock(HitActor, BlockLocation)) {};
	}
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

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f);

	if (bResult) {
		if (HitLocation) {
			*HitLocation = HitResult.Location;
		}
		return HitResult.GetActor();
	}
	return nullptr;
}

bool AAvatar::DestroyHitBlock(AActor* HitActor, FVector& BlockLocation) {
	auto CurrentState = Cast<ASandBoxState>(GetWorld()->GetGameState());

	BlockData* TargetBlockData = nullptr;
	TArray<BlockData>* BlockTable;

	TargetBlock = Cast<ABasicBlock>(HitActor);

	// Decision to type of the type and get the table.
	BlockTable = CurrentState->GetBlockTable(TargetBlock->GetItemID());

	// Get target block data from BlockTable using BlockLocation.
	for (auto iter : *BlockTable) {
		if (iter.Value.Equals(BlockLocation.ToString())) {
			TargetBlockData = &iter;
			break;
		}
	}

	if (TargetBlockData) {
		int targetIndex = TargetBlockData->Key;
		// Remove the block instance.
		if (TargetBlock->MeshInstances->RemoveInstance(targetIndex)) {

			// Remove the block data from BlockTable and refresh the key values.
			for (int i = targetIndex + 1; i < BlockTable->Num(); i++) {
				if (BlockTable->IsValidIndex(i))
					(*BlockTable)[i].Key -= 1;
			}

			if (BlockTable->IsValidIndex(targetIndex))
				BlockTable->RemoveAt(targetIndex);

			// Spawn pick up item.
			TargetBlock->DropItem(FVector(BlockLocation.X + 50.f, BlockLocation.Y + 50.f, BlockLocation.Z + 50.f));

			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			//	FString::Printf(TEXT("After remove... instances:%d table:%d"),
			//		TargetBlock->MeshInstances->GetNumRenderInstances(),
			//		BlockTable->Num()));
			return true;
		}
	}

	return false;
}

void AAvatar::OnHit() {
	AttackAnim();
	AttackTarget();

	if (IsValid(TargetBlock) && !TargetBlock->IsPendingKill()) {
		bIsAtackking = true;
		GetWorldTimerManager().SetTimer(AttackAnimTimer, this, &AAvatar::AttackAnim, 0.3f, true);
		GetWorldTimerManager().SetTimer(BreakBlockTimer, this, &AAvatar::AttackTarget, 0.3f, true);
	}
}

void AAvatar::EndHit() {
	GetWorldTimerManager().ClearTimer(AttackAnimTimer);
	GetWorldTimerManager().ClearTimer(BreakBlockTimer);

	bIsAtackking = false;
	if (IsValid(TargetBlock)) {
		TargetBlock->Restore();
	}
}

void AAvatar::AttackAnim() {
	ABAnim->PlayAttackMontage();
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


	return BlockLocation;
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
}

void AAvatar::ViewChange() {
	switch (CurrentView) {
		// FirstView -> ThirdView(Start view)
	case ViewState::FIRSTPERSON: {
		SpringArm->TargetArmLength = 400.0f;
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

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("First : %s"), *(SpringArm->GetRelativeLocation().ToString())));

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
}

void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAvatar::Jump);
	PlayerInputComponent->BindAction(TEXT("UseItem"), EInputEvent::IE_Pressed, this, &AAvatar::UseItem);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AAvatar::ViewChange);

	//PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AAvatar::Attack);
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