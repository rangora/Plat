// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/Character.h"
#include "avatar/AvatarAnimInstance.h"
#include "avatar/AvatarEquipment.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "item/Interactable.h"
#include "item/AutoPickup.h"
#include "system/AvatarController.h"
#include "UI/ScreenUI.h"
#include "Avatar.generated.h"

enum class ViewState {FIRSTPERSON, THIRDPERSON};

UCLASS()
class PLAT_API AAvatar : public ACharacter {
	GENERATED_BODY()

public:
	AAvatar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	void Attack();
	void UseItem();
	void ViewChange();

	UFUNCTION()
		void PickupItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* 범위 내 대해서 AutoPuckup함수 발동. */
	void CollectAutoPickups();

	/* 범위 내 가장 가까운 상호작용 대상을 체크 한다. */
	void CheckForInteractables();

	float GetHealthValue();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Control..
	void UpDown(float newAxisValue);
	void LeftRight(float newAxisValue);
	void LookUp(float newAxisValue);
	void Turn(float newAxisValue);

	void AttackCheck();

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AAvatarEquipment* Weapon;

private:
	// for UI..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Meta = (AllowPrivateAccess = true))
		float healthValue;

	// for attack function..
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool isAttacking;

	UPROPERTY()
		class UAvatarAnimInstance* ABAnim;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
		UShapeComponent* PickupBox;

	/* Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;

	float attackPower;

	ViewState CurrentView;
};
