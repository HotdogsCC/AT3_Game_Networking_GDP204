// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "ProjectileBase.h"
#include "WizardCharacter.generated.h"


class UHUDUserWidget;
class AProjectileBase;

USTRUCT()
struct FSpell
{
	GENERATED_BODY()

	EFireType FireType;
	float TimeBetweenShots;
	float BurstModeTime;
};

UCLASS()
class WIZARDTESTING_API AWizardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizardCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 DamageTaken);

	UFUNCTION(BlueprintCallable)
	float GetPlayerSpeed() const;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//Input Functions
	UFUNCTION()
	void OnMove(const FInputActionValue& Value);

	UFUNCTION()
	void OnLook(const FInputActionValue& Value);

	UFUNCTION()
	void OnStartSprint();

	UFUNCTION()
	void OnStopSprint();

	UFUNCTION()
	void OnPrimaryFire();
	
	//Input actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryFireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUD_Widget;

	UPROPERTY()
	UHUDUserWidget* HUD_WidgetInstance;
	

	//RPC Primary Fire
	UFUNCTION(Server, Reliable)
	void PrimaryFireServerRPC();

	//RPC Sprint
	UFUNCTION(Server, Reliable)
	void UpdateSprintRPC(float NewSpeed);

	//Projectile Blueprints
	UPROPERTY(EditDefaultsOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileBP;

	//Sprint and walk speeds
	UPROPERTY(EditDefaultsOnly, Category = "Player Attributes", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Player Attributes", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	//How much health the wizard starts with
	UPROPERTY(EditDefaultsOnly, Category = "Player Attributes", meta = (AllowPrivateAccess = "true"))
	int32 MaxHealth;
	
	//How much health they currently have
	UPROPERTY(ReplicatedUsing=OnRepCurrentHealth)
	int32 CurrentHealth;
	
	//temp for instant respawn
	UPROPERTY()
	FVector SpawnLocation;

	//information about the primary spell type
	UPROPERTY()
	FSpell PrimarySpell;

	UPROPERTY(ReplicatedUsing=OnRepCanFire)
	bool bCanFire = true;

	UPROPERTY()
	float CanFireTimer;

	UPROPERTY()
	float CanSecondaryFireTimer;

	UPROPERTY()
	bool bIsBursting = false;
	
	UPROPERTY()
	float CanBurstTimer;

	UPROPERTY()
	uint8 BurstCount;

	UFUNCTION()
	void TickFire(float DeltaTime);

	UFUNCTION()
	void TickBurst(float DeltaTime);
	
	UFUNCTION()
	void SpawnProjectile(UClass* ProjectileToSpawn);

	UFUNCTION()
	void OnRepCurrentHealth();

	UFUNCTION()
	void OnRepCanFire();

};
