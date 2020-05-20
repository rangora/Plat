// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Character.h"
#include "avatar/AvatarAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "item/AutoPickup.h"
#include "item/Equipment.h"
#include "system/AvatarController.h"
#include "blocks/BasicBlock.h"
#include "TimerManager.h"
#include "UI/ScreenUI.h"
#include "Avatar.generated.h"

UCLASS()
class PLAT_API AAvatar : public ACharacter {
	GENERATED_BODY()

public:
	AAvatar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	/* Use the Quickslot item. */
	void UseItem();

	/* Push 'tab' to change view type, first person or thrid person. */
	void ViewChange();

	float GetHealthValue();
	UCameraComponent* GetCameraComponent();

	/* Functions for player equipment. */
	AEquipment* GetWeapon();
	void SetWeapon(AEquipment* NewWeapon, FName ID);
	void DisarmWeapon();

	/* Functions for itme collecting. */
	UFUNCTION()
		void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* 범위 내 대해서 AutoPuckup함수 발동. */
	void CollectAutoPickups();

	/* Block Interaction */
	FVector GetBlockLocation(FVector HitLocation);


protected:
	virtual void BeginPlay() override;

private:
	/* Constant for only used in avatar class. */
	const float BASEATTACKPOWER = 10.f;
	enum class ViewState { FIRSTPERSON, THIRDPERSON };

	/* Movement functions. */
	void UpDown(float newAxisValue);
	void LeftRight(float newAxisValue);
	void LookUp(float newAxisValue);
	void Turn(float newAxisValue);

	/* Attack : Check the validation of target and give damage. */
	void OnHit();
	void AttackTarget();
	void EndHit();

	/* Block Interaction */
	AActor* GetForwardBlock(FVector* HitLocation = nullptr);
	//FVector GetBlockLocation(FVector HitLocation);
	bool DestroyHitBlock(AActor* HitActor, FVector& BlockLocation);

	/* Attack animation. not interaction. */
	void AttackAnim();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	/* Maximum build range for blocks. */
	const float interactRange = 600.f;

private:
	bool bIsAtackking;

	/* Using in  OnHit() function for continous mouse left button down event. */
	FTimerHandle BreakBlockTimer;
	FTimerHandle AttackAnimTimer;

	/* It point block if hit the block on OnHit() function. */
	ABasicBlock* TargetBlock;

	ViewState CurrentView;

	/* PlayerCharacter parameters. */
	float healthValue;
	float AttackRange;
	float AttackRadius;
	float attackPower;
	bool isAttacking;

	UPROPERTY()
		class UAvatarAnimInstance* ABAnim;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AEquipment* Weapon;

	/* Collection sphere for auto pickup. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
};
