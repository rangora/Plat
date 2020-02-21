// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AvatarAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API UAvatarAnimInstance : public UAnimInstance {
	GENERATED_BODY()

public:
	UAvatarAnimInstance();

	virtual void NativeUpdateAnimation(float deltaSeconds) override;

	void PlayAttackMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float currentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool isAttacking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackAnim;
};
