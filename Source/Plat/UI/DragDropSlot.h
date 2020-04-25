// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plat.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/InventorySlot.h"
#include "DragDropSlot.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API UDragDropSlot : public UDragDropOperation {
	GENERATED_BODY()

public:
	class UInventorySlot* From = nullptr;
};
