// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WeaponItem.h"

#include "Player/SurvivalCharacter.h"

UWeaponItem::UWeaponItem()
{
}

bool UWeaponItem::Equip(ASurvivalCharacter* Character)
{
	bool bEquipSuccessful = Super::Equip(Character);

	if (bEquipSuccessful && Character)
	{
		Character->EquipWeapon(this);
	}
	return bEquipSuccessful;
}

bool UWeaponItem::UnEquip(ASurvivalCharacter* Character)
{
	bool bUnEquipSuccessful = Super::UnEquip(Character);

	if (bUnEquipSuccessful && Character)
	{
		Character->UnEquipWeapon();
	}
	return bUnEquipSuccessful;
}
