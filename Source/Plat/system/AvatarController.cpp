// Fill out your copyright notice in the Description page of Project Settings.

#include "AvatarController.h"

AAvatarController::AAvatarController() {
	static ConstructorHelpers::FClassFinder<UPlayerInventory> UI_INVENTORY_C(
		TEXT("/Game/UMG/BP_Inventory.BP_Inventory_C"));

	static ConstructorHelpers::FClassFinder<UScreenUI> UI_SCREEN_C(
		TEXT("/Game/UMG/BP_ScreenUI.BP_ScreenUI_C"));

	if (UI_INVENTORY_C.Succeeded())
		PlayerInventoryClass = UI_INVENTORY_C.Class;

	if (UI_SCREEN_C.Succeeded())
		ScreenUIClass = UI_SCREEN_C.Class;
}

void AAvatarController::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

bool AAvatarController::AddItemToInventory(FName ID) {
	ASandBoxState* GameState = Cast<ASandBoxState>(GetWorld()->GetGameState());

	if (!IsValid(GameState)) 
		return false;
	
	UDataTable* ItemTable = GameState->GetItemDB();
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	int EmptyIndex = PlayerInventoryWidget->FindEmptySlot();

	// Check for empty slot and validation.
	if (ItemToAdd && EmptyIndex >= 0) {
		// Find slot if it is a item that already exists.
		int ExistItemIndex = PlayerInventoryWidget->FindItemInSlots(ItemToAdd->ItemID);
		
		// Exist..
		if (ExistItemIndex >= 0)
			PlayerInventoryWidget->Slots[ExistItemIndex]->ChangeItemCount(1);
		// Doesn't exist..
		else
			PlayerInventoryWidget->Slots[EmptyIndex]->SetNewItem(*ItemToAdd);

		return true;
	}

	return false;
}

void AAvatarController::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(PlayerInventoryWidget)) {
		PlayerInventoryWidget = CreateWidget<UPlayerInventory>(this, PlayerInventoryClass);
	}


	if (!IsValid(ScreenUIWidget)) {
		ScreenUIWidget = CreateWidget<UScreenUI>(this, ScreenUIClass);
		ScreenUIWidget->AddToViewport();
	}
	CreateInventory();
	SetInputMode(FInputModeGameOnly());
}

void AAvatarController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AAvatarController::Interact);
	InputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &AAvatarController::SwitchIventory);
	InputComponent->BindAction(TEXT("Quick1"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick2"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick3"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick4"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
}

void AAvatarController::Interact() {
	if (CurrentInteractable) {
		CurrentInteractable->Interact(this);
	}
}

void AAvatarController::SwitchIventory() {
	if (IsValid(PlayerInventoryWidget)) {
		if (!IsInventoryOpen) {
			PlayerInventoryWidget->AddToViewport(1);
			IsInventoryOpen = true;
			bShowMouseCursor = true;

			FInputModeGameAndUI Mode;
			Mode.SetWidgetToFocus(PlayerInventoryWidget->GetCachedWidget());
			
			SetInputMode(Mode);
		}
		else {
			PlayerInventoryWidget->RemoveFromParent();
			IsInventoryOpen = false;
			bShowMouseCursor = false;

			FInputModeGameOnly Mode;		
			
			SetInputMode(Mode);
		}
	}
}

void AAvatarController::EquipQuickSlot(FKey Key) {
	FString Input = Key.ToString();
	FString ItemName;
	AAvatar* IPlayer = Cast<AAvatar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int inputIndex;

	if (Input.Equals("One")) {
		ItemName = ScreenUIWidget->QuickSlots[0]->LinkedSlot->ItemData.ItemName.ToString();
		inputIndex = 0;
	}
	else if (Input.Equals("Two")) {
		ItemName = ScreenUIWidget->QuickSlots[1]->LinkedSlot->ItemData.ItemName.ToString();
		inputIndex = 1;
	}
	else if (Input.Equals("Three")) {
		ItemName = ScreenUIWidget->QuickSlots[2]->LinkedSlot->ItemData.ItemName.ToString();
		inputIndex = 2;
	}
	else if (Input.Equals("Four")) {
		ItemName = ScreenUIWidget->QuickSlots[3]->LinkedSlot->ItemData.ItemName.ToString();
		inputIndex = 3;
	}

	// If quickSlot of input index is empty. 
	if (ItemName.Equals("None")) {
		if (ScreenUIWidget->GetUsingIndex() != -1) {
			ScreenUIWidget->QuickSlots[ScreenUIWidget->GetUsingIndex()]->ShowBorder(false);
			IPlayer->Weapon->Destroy();
			IPlayer->Weapon = nullptr;
			ScreenUIWidget->SetUsingIndex(-1);
		}
		return;
	}
	// If get same input index with using index, unequip it.
	else if (ScreenUIWidget->GetUsingIndex() == inputIndex) {
		ScreenUIWidget->SetUsingIndex(-1);
		ScreenUIWidget->QuickSlots[inputIndex]->ShowBorder(false);
		IPlayer->Weapon->Destroy();
		IPlayer->Weapon = nullptr;
		return;
	}
	// QuickSlot of input index has an item.
	else {
		// Remove weapon's static mesh if there are pre-behavior.
		if (ScreenUIWidget->GetUsingIndex() > -1) {
			ScreenUIWidget->QuickSlots[ScreenUIWidget->GetUsingIndex()]->ShowBorder(false);
			IPlayer->Weapon->Destroy();
			IPlayer->Weapon = nullptr;
		}

		ScreenUIWidget->SetUsingIndex(inputIndex);
		ScreenUIWidget->QuickSlots[inputIndex]->ShowBorder(true);
		FString Path = "/Game/resources/blocks/";
		Path.Append(ItemName + "." + ItemName);

		auto WeaponActor = GetWorld()->SpawnActor<AAvatarEquipment>(AAvatarEquipment::StaticClass(),
			FVector::ZeroVector, FRotator::ZeroRotator);

		WeaponActor->SetWeaponStaticMesh(Path, ItemName);

		if (IsValid(WeaponActor)) {
			FName WeaponSocket(TEXT("hands_rSocket"));

			WeaponActor->AttachToComponent(IPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

			IPlayer->Weapon = WeaponActor;
		}
	}
}

void AAvatarController::CloseInventory() {
	if (IsInventoryOpen) {
		PlayerInventoryWidget->RemoveFromParent();
		IsInventoryOpen = false;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameAndUI());
	}
}

void AAvatarController::CreateInventory() {
	// This Init function is blueprintImplementableEvent.
	PlayerInventoryWidget->InitInventory();
	ScreenUIWidget->InitQuickSlots();

	ScreenUIWidget->LinkSlot(0, PlayerInventoryWidget->Slots[12]);
	ScreenUIWidget->LinkSlot(1, PlayerInventoryWidget->Slots[13]);
	ScreenUIWidget->LinkSlot(2, PlayerInventoryWidget->Slots[14]);
	ScreenUIWidget->LinkSlot(3, PlayerInventoryWidget->Slots[15]);
}