// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "GameFramework/PlayerController.h"
#include "item/Interactable.h"
#include "item/InventoryItem.h"
#include "system/SandBoxState.h"
#include "avatar/Avatar.h"
#include "avatar/AvatarEquipment.h"
#include "UI/PlayerInventory.h"
#include "InputCoretypes.h"
#include "AvatarController.generated.h"

/**
 *
 */
UCLASS()
class PLAT_API AAvatarController : public APlayerController {
	GENERATED_BODY()

public:
	AAvatarController();
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
		bool AddItemToInventory(FName ID);

	UFUNCTION(BlueprintCallable)
		void CloseInventory();

	void CreateInventory();

	

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Interact();

private:
	void SwitchIventory();
	void EquipQuickSlot(FKey Key);

public:
	UPROPERTY(VisibleAnywhere)
		class TSubclassOf<class UScreenUI> ScreenUIClass;

	UPROPERTY(VisibleAnywhere)
		UScreenUI* ScreenUIWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class TSubclassOf<class UPlayerInventory> PlayerInventoryClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPlayerInventory* PlayerInventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AInteractable* CurrentInteractable;

	bool IsInventoryOpen = false;

private:
	AAvatarEquipment* WeaponTable;


	
};
