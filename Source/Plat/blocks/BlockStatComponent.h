// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "system/ABGameInstance.h"
#include "Components/ActorComponent.h"
#include "BlockStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAT_API UBlockStatComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UBlockStatComponent();
		
	FOnHPIsZeroDelegate OnHPIsZero;
	
	struct FBlockData* CurrentStatData = nullptr;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

	
	void SetAttribute();
	void SetDamage(float NewDamage);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;


private:
	
};
