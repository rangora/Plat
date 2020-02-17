// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

UCLASS()
class PLAT_API AAvatar : public ACharacter {
	GENERATED_BODY()

private:
	void UpDown(float newAxisValue);
	void LeftRight(float newAxisValue);
	void LookUp(float newAxisValue);
	void Turn(float newAxisValue);
	

protected:
	virtual void BeginPlay() override;
	void SetControlMode(int32 controlMode);

public:	
	AAvatar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;
};
