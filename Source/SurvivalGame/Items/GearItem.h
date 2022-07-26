// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquippableItem.h"
#include "GearItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SURVIVALGAME_API UGearItem : public UEquippableItem
{
	GENERATED_BODY()

public:

	UGearItem();

	virtual bool Equip(ASurvivalCharacter* Character) override;
	virtual bool UnEquip(ASurvivalCharacter* Character) override;
	
	// Skeletal mesh for the this gear
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gear")
	class USkeletalMesh* Mesh;

	// Optional Material Instance to apply to the gear
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gear")
	class UMaterialInstance* MaterialInstance;
	
	// Amount of defence this item provides, 0.2 = 20%, 1 = 100%, less damage taken
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gear", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float DamageDefenceMultiplier;
	
};
