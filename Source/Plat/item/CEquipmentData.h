// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "item/CBaseItemData.h"
#include "CEquipmentData.generated.h"


UENUM(BlueprintType)
enum class EEQuipmentDetail : uint8 {
	NONE		UMETA(DisplayName = "None"),
	WEAPON		UMETA(DisplayName = "Weapon"),
	UTILITY		UMETA(DisplayName = "Utility")
};


USTRUCT(BlueprintType)
struct FEquipmentData : public FBaseItemData {
	GENERATED_BODY()

public:
	FEquipmentData();
	FEquipmentData(const FEquipmentData& Item);

	virtual void Clear() override;
	void SetThisItem(const FEquipmentData& Item);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEQuipmentDetail DetailType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AdventageTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AdDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

};