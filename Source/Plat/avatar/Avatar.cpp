// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "DrawDebugHelpers.h"

AAvatar::AAvatar() {
	PrimaryActorTick.bCanEverTick = true;
	isAttacking = false;
	Weapon = nullptr;
	AttackRange = 200.;
	AttackRadius = 50.;
	attackPower = 50.;
	healthValue = 1.0;

	// Create SpringArmComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// Create CameraComponents
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	CameraComponent->SetupAttachment(SpringArm);

	// Create Actor Character
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Avatar(
		TEXT("/Game/resources/character/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_Avatar.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_Avatar.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AVATAR_ANIM(
		TEXT("/Game/resources/character/AvatarAnimBlueprint.AvatarAnimBlueprint_C"));
	if (AVATAR_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(AVATAR_ANIM.Class);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)); // Set pivot
	
	// Weapon
	Weapon = nullptr;
	
	// Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Avatar"));

	// jump
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// create the collection sphere for auto pick up system
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);
	
	// view
	CurrentView = ViewState::FIRSTPERSON;
	ViewChange();
}

void AAvatar::UpDown(float newAxisValue) {
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

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

void AAvatar::AttackCheck() {
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult) {
		if (HitResult.Actor.IsValid())
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(attackPower, DamageEvent, GetController(), this);
		this->healthValue -= 0.1f;
	}
}

void AAvatar::Attack() {
	if (isAttacking) return;

	ABAnim->PlayAttackMontage();
	isAttacking = true;

	//FVector Start = Camera->GetComponentLocation();
	//FVector end = (Camera->GetForwardVector() * 1000) + Start;

	//DrawDebugLine(GetWorld(), Start, end, FColor::Red, false, 3.f, 0, 10.f);

	AttackCheck();
}

void AAvatar::UseItem() {
	float Range = 350.f;
	auto IController = Cast<AAvatarController>(GetController());
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = (CameraComponent->GetForwardVector() * Range) + Start;
	
	int index = IController->ScreenUIWidget->GetUsingIndex();
	
	// Check quick slot valiable.
	if (index < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
			FString::Printf(TEXT("No Index")));
		return;
	}
	FString BlockName = IController->ScreenUIWidget->QuickSlots[index]->LinkedSlot->ItemData.ItemName.ToString();
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//	FString::Printf(TEXT("BlockName : %s"), *BlockName));

	FHitResult CollisionResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
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
		FVector CharaLocation = CameraComponent->GetComponentLocation();
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


		CharaLocation.X = FMath::RoundToInt(CharaLocation.X / 100.f) * 100;
		CharaLocation.Y = FMath::RoundToInt(CharaLocation.Y / 100.f) * 100;
		CharaLocation.Z = int(CharaLocation.Z) / 100 * 100;

		// Check collision between player and block will be deployed.
		if (CharaLocation.Equals(_deployLocation)) {
			return;
		}

		// Build the block.
		else {
			FString BP_GrassPath = "/Game/Blueprints/BP_" 
				+ BlockName + "." + "BP_" + BlockName + "_C"; // Dirt.BP_Dirt_C";
			UClass* BP_Block = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BP_GrassPath));

			if (IsValid(BP_Block)) {
				GetWorld()->SpawnActor<ABasicBlock>(BP_Block,
					_deployLocation, FRotator::ZeroRotator);
			
				IController->ScreenUIWidget->QuickSlots[index]->UseItem();
			}
			return;
		}
	}
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

void AAvatar::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	isAttacking = false;
}

void AAvatar::CheckForInteractables() {
	// Create a LineTrace to check for a hit
	FHitResult HitResult;

	int32 Range = 1000;

	FVector StartTrace;
	FVector EndTrace;

	StartTrace = CameraComponent->GetComponentLocation();
	EndTrace = (CameraComponent->GetForwardVector() * Range) + StartTrace;


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	AAvatarController* IController = Cast<AAvatarController>(GetController());

	if (IController) {
		// Check if something is hit
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams)) {
			// Cast the actor to AInteractable
			AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());
			// If the cast is successful
			if (Interactable) {
				IController->CurrentInteractable = Interactable;
				return;
			}
		}
		IController->CurrentInteractable = nullptr;
	}
}

float AAvatar::GetHealthValue() {
	return healthValue;
}

void AAvatar::BeginPlay() {
	Super::BeginPlay();
}

void AAvatar::ViewChange() {
	switch (CurrentView) {
	case ViewState::FIRSTPERSON: {
			SpringArm->TargetArmLength = 400.0f;
			SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
			SpringArm->bUsePawnControlRotation = true; 

			bUseControllerRotationYaw = false;
			bUseControllerRotationPitch = false;
			bUseControllerRotationRoll = false;

			GetMesh()->bOwnerNoSee = false;
			GetMesh()->MarkRenderStateDirty();

			GetCharacterMovement()->bOrientRotationToMovement = true; 
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

			CurrentView = ViewState::THIRDPERSON;
			break;
		}
	case ViewState::THIRDPERSON: {	
			SpringArm->SetRelativeLocation(FVector(23.f, 0.f, 67.f));
			SpringArm->TargetArmLength = 0.f;
			bUseControllerRotationYaw = true;
			GetMesh()->bOwnerNoSee = true;
			GetMesh()->MarkRenderStateDirty();

			CurrentView = ViewState::FIRSTPERSON;
			break;
		}
	}
}

void AAvatar::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CollectAutoPickups();
	CheckForInteractables();
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAvatar::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AAvatar::Attack);
	PlayerInputComponent->BindAction(TEXT("UseItem"), EInputEvent::IE_Pressed, this, &AAvatar::UseItem);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AAvatar::ViewChange);

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