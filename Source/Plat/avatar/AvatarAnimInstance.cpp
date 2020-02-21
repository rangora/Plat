// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarAnimInstance.h"

UAvatarAnimInstance::UAvatarAnimInstance() {
	currentPawnSpeed = 0.0f;
	isInAir = false;
	isAttacking = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(
		TEXT("/Game/resources/character/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));

	if (ATTACK_MONTAGE.Succeeded())
		AttackAnim = ATTACK_MONTAGE.Object;
}

void UAvatarAnimInstance::NativeUpdateAnimation(float deltaSeconds) {
	Super::NativeUpdateAnimation(deltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn)) {
		currentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
			isInAir = Character->GetMovementComponent()->IsFalling();
		
	}
}

void UAvatarAnimInstance::PlayAttackMontage() {
	Montage_Play(AttackAnim, 1.0f);
	//BlueprintBeginPlay();
}
