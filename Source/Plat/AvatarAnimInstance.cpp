// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarAnimInstance.h"

UAvatarAnimInstance::UAvatarAnimInstance() {
	currentPawnSpeed = 0.0f;
	isInAir = false;
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
