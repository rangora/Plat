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
	int EmptyIndex = PlayerInventoryWidget->FindEmptySlot();

	// Check for empty slot and validation.
	if (EmptyIndex >= 0) {
		// Find slot if it is a item that already exists.
		int ExistItemIndex = PlayerInventoryWidget->FindItemInSlots(ID);

		// Exist..
		if (ExistItemIndex >= 0) {
			PlayerInventoryWidget->Slots[ExistItemIndex]->AddItemCount(1);
			PlayerInventoryWidget->Slots[ExistItemIndex]->RefreshQuickSlot();
		}
		// Doesn't exist..
		else
			PlayerInventoryWidget->Slots[EmptyIndex]->SetNewItem(ID);

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
		//ScreenUIWidget->AddToViewport();
	}
	CreateInventory();
	SetInputMode(FInputModeGameOnly());
}

void AAvatarController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("OpenInventory"), EInputEvent::IE_Pressed, this, &AAvatarController::SwitchIventory);
	InputComponent->BindAction(TEXT("Quick1"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick2"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick3"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick4"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick5"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick6"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick7"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
	InputComponent->BindAction(TEXT("Quick8"), EInputEvent::IE_Pressed, this, &AAvatarController::EquipQuickSlot);
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
	int inputIndex = -1;

	if (Input.Equals("One")) {
		ItemName = ScreenUIWidget->QuickSlots[0]->LinkedSlot->ItemName;
		inputIndex = 0;
	}
	else if (Input.Equals("Two")) {
		ItemName = ScreenUIWidget->QuickSlots[1]->LinkedSlot->ItemName;
		inputIndex = 1;
	}
	else if (Input.Equals("Three")) {
		ItemName = ScreenUIWidget->QuickSlots[2]->LinkedSlot->ItemName;
		inputIndex = 2;
	}
	else if (Input.Equals("Four")) {
		ItemName = ScreenUIWidget->QuickSlots[3]->LinkedSlot->ItemName;
		inputIndex = 3;
	}
	else if (Input.Equals("Five")) {
		ItemName = ScreenUIWidget->QuickSlots[4]->LinkedSlot->ItemName;
		inputIndex = 4;
	}
	else if (Input.Equals("Six")) {
		ItemName = ScreenUIWidget->QuickSlots[5]->LinkedSlot->ItemName;
		inputIndex = 5;
	}
	else if (Input.Equals("Seven")) {
		ItemName = ScreenUIWidget->QuickSlots[6]->LinkedSlot->ItemName;
		inputIndex = 6;
	}
	else if (Input.Equals("Eight")) {
		ItemName = ScreenUIWidget->QuickSlots[7]->LinkedSlot->ItemName;
		inputIndex = 7;
	}

	// If quickSlot of input index is empty.
	if (ItemName.Equals("None")) {
		if (ScreenUIWidget->GetUsingIndex() != -1) {
			ScreenUIWidget->QuickSlots[ScreenUIWidget->GetUsingIndex()]->ShowBorder(false);
			IPlayer->DisarmWeapon();
			ScreenUIWidget->SetUsingIndex(-1);
		}
		return;
	}
	// If get same input index with using index, unequip it.
	else if (ScreenUIWidget->GetUsingIndex() == inputIndex) {
		ScreenUIWidget->SetUsingIndex(-1);
		ScreenUIWidget->QuickSlots[inputIndex]->ShowBorder(false);
		IPlayer->DisarmWeapon();
		return;
	}
	// QuickSlot of input index has an item.
	else {
		// Remove weapon's static mesh if there are pre-behavior.
		if (ScreenUIWidget->GetUsingIndex() > -1) {
			ScreenUIWidget->QuickSlots[ScreenUIWidget->GetUsingIndex()]->ShowBorder(false);
			IPlayer->DisarmWeapon();
		}

		// Only equipment visible static mesh.
		ScreenUIWidget->SetUsingIndex(inputIndex);
		ScreenUIWidget->QuickSlots[inputIndex]->ShowBorder(true);
		FString Path = "/Game/resources/equipment/";
		//FString Path = "/Game/resources/blocks/";
		Path.Append(ItemName + "." + ItemName);

		auto WeaponActor = GetWorld()->SpawnActor<AEquipment>(AEquipment::StaticClass(),
			FVector::ZeroVector, FRotator::ZeroRotator);

		// If a item is equiptable, set as weapon.
		if (WeaponActor->SetWeaponStaticMesh(Path, ItemName)) {
			FName WeaponSocket(TEXT("hands_rSocket"));

			WeaponActor->AttachToComponent(IPlayer->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

			FName ItemID = ScreenUIWidget->QuickSlots[ScreenUIWidget->GetUsingIndex()]->LinkedSlot->ItemID;

			// Give atrributes to player in accordance with equipment.
			IPlayer->SetWeapon(WeaponActor, ItemID);
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

	for (int i = 0; i < QuickSlotSize; i++)
		ScreenUIWidget->LinkSlot(i, PlayerInventoryWidget->Slots[FirstLinkedIndex + i]);;
}