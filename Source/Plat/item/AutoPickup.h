// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "AutoPickup.generated.h"

/**
This class is used to implement items which are picked up automatically
when the player is in pickup range to the actors.
*/

class APlayerController;

UCLASS()
class PLAT_API AAutoPickup : public AActor {
	GENERATED_BODY()

public:
	AAutoPickup();

	virtual void Tick(float DeltaTime) override;
	virtual void Collect_Implementation(APlayerController* Controller);

	FName GetItemID();

	UFUNCTION(BlueprintNativeEvent)
		void Collect(APlayerController* Controller);

protected:
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* Rotation;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

private:
	float rotateSpeed;

	bool bIsCollecting = false;
};
