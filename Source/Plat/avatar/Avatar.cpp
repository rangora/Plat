// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar.h"

// Sets default values
AAvatar::AAvatar() {
 	PrimaryActorTick.bCanEverTick = true;
	isAttacking = false;
	AttackRange = 200.;
	AttackRadius = 50.;
	attackPower = 50.;


	// init..
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Avatar"));

	// place..
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// actor allocate..
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Avatar(
		TEXT("/Game/resources/character/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_Avatar.Succeeded()) 
		GetMesh()->SetSkeletalMesh(SK_Avatar.Object);

	// anim allocate..
	static ConstructorHelpers::FClassFinder<UAnimInstance> AVATAR_ANIM(
		TEXT("/Game/resources/character/AvatarAnimBlueprint.AvatarAnimBlueprint_C"));
	if (AVATAR_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(AVATAR_ANIM.Class);

	// jump..
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// view..
	SetControlMode(0);
}

void AAvatar::UpDown(float newAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), newAxisValue);
}

void AAvatar::LeftRight(float newAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), newAxisValue);
}

void AAvatar::LookUp(float newAxisValue) {
	AddControllerPitchInput(newAxisValue);
}

void AAvatar::Turn(float newAxisValue) {
	AddControllerYawInput(newAxisValue);
}

void AAvatar::AttackCheck() {
	ABLOG(Warning, TEXT("Attack_Check"));

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
	
	}

}

void AAvatar::Attack() {
	if (isAttacking) return;

	ABAnim->PlayAttackMontage();
	isAttacking = true;

	AttackCheck();
}

void AAvatar::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	isAttacking = false;
}

void AAvatar::BeginPlay() {
	Super::BeginPlay();
}

void AAvatar::SetControlMode(int32 controlMode) {
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bInheritYaw = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

// Called every frame
void AAvatar::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAvatar::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AAvatar::Attack);

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

