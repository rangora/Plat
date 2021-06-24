// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "avatar/AvatarAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "item/AutoPickup.h"
#include "item/Equipment.h"
#include "blocks/BasicBlock.h"
#include "TimerManager.h"
#include "UI/ScreenUI.h"
#include "WorldCreater.h"
#include "Partial.h"
#include "WorldMap.h"
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
	bool DeployBlock(FName ItemID, FString BlockName);

protected:
	virtual void BeginPlay() override;

private:
	/* Constant for only used in avatar class. */
	const float BASEATTACKPOWER = 20.f;
	enum class ViewState { FIRSTPERSON, THIRDPERSON };

	/* Movement functions. */
	void UpDown(float newAxisValue);
	void LeftRight(float newAxisValue);
	void LookUp(float newAxisValue);
	void Turn(float newAxisValue);
	void NoCollision();

	/* Attack : Check the validation of target and give damage. */
	void OnHit();
	void AttackTarget();
	float GetAttackDamage();
	void EndHit();
	void ResetTargetBlock();

	/* Block interaction. */
	void SetTargetBlock(AActor* HitBlock, FVector BlockVector);
	FVector GetBlockLocation(FVector HitLocation);
	FVector GetDeployLocation(FVector HitVector);
	AActor* GetForwardBlock(FVector* HitLocation = nullptr);
	bool DestroyTargetBlock();
	bool IsVectorOverlapped(FVector DeployVector);

	/* Attack animation. not interaction. */
	void AttackAnim();

	/* Map loading wrapper function. */
	void LoadMaps();
	void CreateUnderground();
	TArray<AWorldMap*> GetNearMap();
	TArray<APartial*> GetNearUndergroundPartials();

	/* Debug */
	void ShowCurrentVector();

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

	/* Map Loading Timer. */
	FTimerHandle MapLoadTimer;
	FTimerHandle UndergroundCreateTimer;

	/* It point a hit block in OnHit() function. */
	ABasicBlock* TargetBlock = nullptr;
	APartial* CurrentPartial = nullptr;
	TArray<BlockData>* CurrentTable = nullptr;

	/* Hit location of a block. */
	FVector HitLocation;

	/* View type. */
	ViewState CurrentView;

	/* Unique worldCreater */
	AWorldCreater* Creater;

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

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CreateUndergroundSphere;

	class USphereComponent* CreateMapSphere;
};
