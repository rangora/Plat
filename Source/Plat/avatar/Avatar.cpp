// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "DrawDebugHelpers.h"

AAvatar::AAvatar() {
	PrimaryActorTick.bCanEverTick = true;
	isAttacking = false;
	AttackRange = 200.;
	interactRange = 600.;
	AttackRadius = 50.;
	attackPower = 50.;
	healthValue = 1.0;

	// Create SpringArmComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
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
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult) {
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(attackPower, DamageEvent, GetController(), this);
		this->healthValue -= 0.1f;
	}
}

void AAvatar::Attack() {
	if (isAttacking) return;

	ABAnim->PlayAttackMontage();
	isAttacking = true;

	AttackCheck();
}

void AAvatar::UseItem() {
	auto IController = Cast<AAvatarController>(GetController());
	int index = IController->ScreenUIWidget->GetUsingIndex();
	
	// Check quick slot valiable.
	if (index < 0) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
			FString::Printf(TEXT("No Index")));
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

AAvatarEquipment* AAvatar::GetWeapon() {
	return Weapon;
}

UCameraComponent* AAvatar::GetCameraComponent() {
	return CameraComponent;
}

void AAvatar::SetWeapon(AAvatarEquipment* NewWeapon) {
	Weapon = NewWeapon;
}

void AAvatar::BeginPlay() {
	Super::BeginPlay();
}

void AAvatar::ViewChange() {
	switch (CurrentView) {
		// FirstView -> ThirdView
	case ViewState::FIRSTPERSON: {
			SpringArm->TargetArmLength = 400.0f;
			SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
			SpringArm->bUsePawnControlRotation = true; 
			AttackRange += SpringArm->TargetArmLength;

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
		// ThriedView -> FirstView
	case ViewState::THIRDPERSON: {	
			AttackRange -= SpringArm->TargetArmLength;
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

void AAvatar::PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AAutoPickup* Item = Cast<AAutoPickup>(OtherActor);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
		FString::Printf(TEXT("ID:%s"), *Item->GetItemID().ToString()));

	if (IsValid(Item)) {
		auto IController = Cast<AAvatarController>(GetController());
		if (IController->AddItemToInventory(Item->GetItemID()))
			Item->Destroy();
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
