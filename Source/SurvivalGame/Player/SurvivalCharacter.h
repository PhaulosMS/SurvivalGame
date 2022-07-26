// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalCharacter.generated.h"

enum class EEquippableSlot : uint8; // Forward Declaration


USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData()
	{
		ViewedInteractionComponent = nullptr;
		LastInteractionCheckTime = 0.0f;
		bInteractHeld = false;
	}

	UPROPERTY()
	class UInteractionComponent* ViewedInteractionComponent;

	UPROPERTY()
	float LastInteractionCheckTime;

	UPROPERTY()
	bool bInteractHeld;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemsChanged, const EEquippableSlot, Slot, const class UEquippableItem*, Item);

UCLASS()
class SURVIVALGAME_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalCharacter();

	// The mesh to have equipped if we don't have an item equipped - IE the bare skin meshes
	UPROPERTY(BlueprintReadOnly, Category = Mesh)
	TMap<EEquippableSlot, USkeletalMesh*> NakedMeshes;

	// The player's body meshes
	UPROPERTY(BlueprintReadOnly, Category = "Mesh")
	TMap<EEquippableSlot, USkeletalMeshComponent*> PlayerMeshes;

	// Interaction component used to allow other players to loot us when we have died
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* LootPlayerInteraction;

	bool IsInteracting() const;
	float GetRemainingInteractTime() const;

	// Our Players inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* PlayerInventory;

	// Items

	/** [Server] Use an item from our Inventory */
	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem(class UItem* Item);

	/** [Server] Drop an Item */
	UFUNCTION(BlueprintCallable, Category = "Items")
	void DropItem(class UItem* Item, const int32 Quantity);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropItem(class UItem* Item, const int32 Quantity);

	/** We need this because the pickups use a blueprint base class */
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class APickup> PickupClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void Restart() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

public:

	UFUNCTION(BlueprintCallable)
	void SetLootSource(class UInventoryComponent* NewLootSource);

	UFUNCTION(BlueprintCallable, Category = "Looting")
	bool IsLooting() const;

protected:

	// Begin being looted by a player
	UFUNCTION()
	void BeginLootingPlayer(class ASurvivalCharacter* Character);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void ServerSetLootSource(class UInventoryComponent* NewLootSource);

	// The inventory we are currently looting from
	UPROPERTY(ReplicatedUsing = OnRep_LootSource, BlueprintReadOnly)
	class UInventoryComponent* LootSource;

	UFUNCTION()
	void OnLootSourceOwnerDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnRep_LootSource();

public:

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;

	UPROPERTY()
	float WalkSpeed;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
	bool bSprinting;

	UFUNCTION(BlueprintCallable, Category = "Looting")
	void LootItem(class UItem* ItemToGive);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLootItem(class  UItem* ItemToLoot);

	// Locally start and stop sprinting functions
	bool CanSprint() const;
	
	void StartSprinting();
	void StopSprinting();

	void SetSprinting(const bool bNewSprinting);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(const bool bNewSprinting);
	
protected:

	void StartCrouching();
	void StopCrouching();
	void MoveForward(float Val);
	void MoveBackwards(float Val);
	void MoveRight (float Val);
	void LookUp(float Val);
	void Turn (float Val);

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionCheckDistance;

	void PerformInteractionCheck();
	void CouldntFindInteractable();
	void FoundNewInteractable(UInteractionComponent* Interactable);

	void BeginInteract();
	void EndInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginInteract();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndInteract();

	void Interact();

	UPROPERTY()
	FInteractionData  InteractionData;

	FTimerHandle TimerHandle_Interact;

	FORCEINLINE class UInteractionComponent* GetInteractable() const { return InteractionData.ViewedInteractionComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Items")
	TMap<EEquippableSlot, class UEquippableItem*> EquippedItems;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE TMap<EEquippableSlot, UEquippableItem*> GetEquippedItems() const { return EquippedItems; }

	UFUNCTION(BlueprintPure)
	class USkeletalMeshComponent* GetSlotSkeletalMeshCmponent(const EEquippableSlot Slot);

	UPROPERTY(BlueprintAssignable, Category = "Items")
	FOnEquippedItemsChanged OnEquippedItemsChanged;

	bool EquipItem(class UEquippableItem* Item);
	bool UnEquipItem(class UEquippableItem* Item);

	void EquipGear(class UGearItem* Gear);
	void UnEquipGear(const EEquippableSlot Slot);

	void EquipWeapon(class UWeaponItem* WeaponItem);
	void UnEquipWeapon();

	void StartReload();
	
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	FORCEINLINE class AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	// Modify the players Health by either a negative or positive amount return the amount of health actually removed
	float ModifyHealth(const float Delta);

	UFUNCTION()
	void OnRep_Health(float OldHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthModified(const float HealthDelta);

	

protected:

	UFUNCTION(Server, Reliable)
	void ServerUseThrowable();

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastPlayThrowableTossFX(UAnimMontage* Montage);

	class UThrowableItem* GetThrowable()  const;
	void UseThrowable();
	void SpawnThrowable();
	bool CanUseThrowable() const;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void StartFire();
	void StopFire();

	void BeginMeleeAttack();

	UFUNCTION(Server, Reliable)
	void ServerProcessMeleeHit(const FHitResult& MeleeHit);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayMeleeFX();
	
	UPROPERTY()
	float LastMeleeAttackTime;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	float MeleeAttackDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	float MeleeAttackDamage;

	

	UPROPERTY(EditDefaultsOnly, Category = "Melee")
	class UAnimMontage* MeleeAttackMontage;

protected:

	void Suicide(struct FDamageEvent const& DamageEvent, const AActor* DamageCauser);
	void KilledByPlayer(struct FDamageEvent const& DamageEvent, class ASurvivalCharacter* Character, const AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing = OnRep_Killer)
	class ASurvivalCharacter* Killer;

	UFUNCTION()
	void OnRep_Killer();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess))
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* HelmetMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* ChestMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* LegsMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* FeetMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* VestMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* HandsMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* BackpackMesh;

protected:
	bool CanAim() const;
	void StartAiming();
	void StopAiming();

	// Aiming
	void SetAiming(const bool bNewAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(const bool bNewAiming);

	UPROPERTY(Transient, Replicated);
	bool bIsAiming;

public:

	FORCEINLINE bool IsAlive() const { return Killer == nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE bool IsAiming() const { return bIsAiming; }
	
	
};


