// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/GearItem.h"

#include "Player/SurvivalCharacter.h"

UGearItem::UGearItem()
{
	DamageDefenceMultiplier = 0.1f; // 10% default damage reduction
}

bool UGearItem::Equip(ASurvivalCharacter* Character)
{
	const bool bEquipSuccessful = Super::Equip(Character);

	if (bEquipSuccessful && Character)
	{
		Character->EquipGear(this);
	}
	return bEquipSuccessful;
}

bool UGearItem::UnEquip(ASurvivalCharacter* Character)
{
	const bool bUnEquipSuccessful = Super::UnEquip(Character);

	if (bUnEquipSuccessful && Character)
	{
		Character->UnEquipGear(Slot);
	}
	return bUnEquipSuccessful;
}
