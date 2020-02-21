// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "CoreMinimal.h"
#include "AvatarAnimInstance.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

UCLASS()
class PLAT_API AAvatar : public ACharacter {
	GENERATED_BODY()

private:
	float attackPower;
	// Control..
	void UpDown(float newAxisValue);
	void LeftRight(float newAxisValue);
	void LookUp(float newAxisValue);
	void Turn(float newAxisValue);

	void AttackCheck();

	// for attack function..
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool isAttacking;
	
	UPROPERTY()
		class UAvatarAnimInstance* ABAnim;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;
	void SetControlMode(int32 controlMode);

public:	
	AAvatar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	void Attack();
	
	

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;
	
};
