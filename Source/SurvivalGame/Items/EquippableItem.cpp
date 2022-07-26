// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EquippableItem.h"
#include "UnrealNetwork.h"
#include "Player/SurvivalCharacter.h"
#include "Components/InventoryComponent.h"

#define LOCTEXT_NAMESPACE "EquippableItem"

UEquippableItem::UEquippableItem()
{
	bStackable = false;
	bEquppied = false;
	UseActionText = LOCTEXT("ItemUseActionText", "Equip");
}

void UEquippableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquippableItem, bEquppied);
}

void UEquippableItem::Use(ASurvivalCharacter* Character)
{
	if (Character && Character->HasAuthority())
	{
		if (Character->GetEquippedItems().Contains(Slot) && !bEquppied)
		{
			UEquippableItem* AlreadyEquippedItem = *Character->GetEquippedItems().Find(Slot);
			AlreadyEquippedItem->SetEquipped(false);
		}
		SetEquipped(!IsEquipped());
	}
}

bool UEquippableItem::Equip(ASurvivalCharacter* Character)
{
	if (Character)
	{
		return Character->EquipItem(this);
	}
	return false;
}

bool UEquippableItem::UnEquip(ASurvivalCharacter* Character)
{
	if (Character)
	{
		return Character->UnEquipItem(this);
	}
	return false;
}

bool UEquippableItem::ShouldShowInInventory() const
{
	return !bEquppied;
}

void UEquippableItem::AddedToInventory(UInventoryComponent* Inventory)
{
	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(Inventory->GetOwner()))
	{
		if (Character && !Character->IsLooting())
		{
			if (!Character->GetEquippedItems().Contains(Slot))
			{
				SetEquipped(true);
			}
		}
	}
}

void UEquippableItem::SetEquipped(bool bNewEquipped)
{
	bEquppied = bNewEquipped;
	EquipStatusChanged();
	MarkDirtyForReplication();
}

void UEquippableItem::EquipStatusChanged()
{
	if (ASurvivalCharacter* Character = Cast<ASurvivalCharacter>(GetOuter()))
	{
		if (bEquppied)
		{
			Equip(Character);
		}
		else
		{
			UnEquip(Character);
		}
	}
	// Tell UI to update
	OnItemModified.Broadcast();
}

#undef LOCTEXT_NAMESPACE