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

	/* ���� �� ���ؼ� AutoPuckup�Լ� �ߵ�. */
	void CollectAutoPickups();

	/* ���� �� ���� ����� ��ȣ�ۿ� ����� üũ �Ѵ�. */
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

	/* Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;

	float attackPower;

	ViewState CurrentView;
};
