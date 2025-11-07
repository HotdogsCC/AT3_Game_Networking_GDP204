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
	
	
};

UCLASS()
class WIZARDTESTING_API AWizardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizardCharacter();
	
	// Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called to set which variables replicate over the network
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// returns the speed of the player; used 
	UFUNCTION(BlueprintCallable)
	float GetPlayerSpeed() const;

	UFUNCTION(BlueprintCallable)
	bool GetShouldThrowAnimation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//the mesh for the fireball
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;

	//Projectile Blueprint
	UPROPERTY(EditDefaultsOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true")) 
	TSubclassOf<AProjectileBase> ProjectileBP;

	//Sprint and walk speeds
	UPROPERTY(EditDefaultsOnly, Category = "Player Attributes", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Player Attributes", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	// --- Input actions ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	// widget to create for the hud
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUD_Widget;

private:
	//updates logic for fire timings
	UFUNCTION()
	void TickFire(float DeltaTime);

	//creates a projectile 
	UFUNCTION()
	void SpawnProjectile(UClass* ProjectileToSpawn);

	//updates the hud to reflect the current firing state
	UFUNCTION()
	void UpdateHUD();
	
	// --- Input functions ---
	UFUNCTION()
	void OnMove(const FInputActionValue& Value);

	UFUNCTION()
	void OnLook(const FInputActionValue& Value);

	UFUNCTION()
	void OnStartSprint();

	UFUNCTION()
	void OnStopSprint();

	UFUNCTION()
	void OnFire();
	
	//RPC Primary Fire
	UFUNCTION(Server, Reliable)
	void PrimaryFireServerRPC();

	//RPC Sprint
	UFUNCTION(Server, Reliable)
	void UpdateSprintRPC(float NewSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void PlayThrowAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void FireRecharged();

	//when bCanFire updates over the network
	UFUNCTION()
	void OnRepCanFire();

	//whether this player can currently shoot
	UPROPERTY(ReplicatedUsing=OnRepCanFire)
	bool bCanFire = true;

	//whether the animation should play the throw animation
	UPROPERTY()
	bool bShouldThrowAnimation = false;
	
	//the time between each projectile being fired
	UPROPERTY()
	float TimeBetweenShots;

	//the elapsed time since the last shot
	UPROPERTY()
	float CanFireTimer;

	//reference to the current hud instance
	UPROPERTY()
	UHUDUserWidget* HUD_WidgetInstance = nullptr;

};
