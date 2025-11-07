// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"


class UPointLightComponent;


UCLASS()
class WIZARDTESTING_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// called to enable collisions
	UFUNCTION()
	void StartDetectingCollisions() const;

	// called to get the time between each shot
	UFUNCTION()
	float GetTimeBetweenShots() const;

	// used for where the spell should travel to 
	void SetTarget(const FVector& InTargetLocation);

	// sets a reference of who spawned us
	UFUNCTION()
	void SetWizardOwner(AActor* Wizard);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//the mesh of this projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	//the light object in the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPointLightComponent* LightComponent;

	//how long the projectile is alive for
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DespawnTime = 3.0f;

	//how fast the projectile is
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 5000.0f;

	//the curviness of the projectile
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Curviness = 0.0f;

	//time between each projectile to spawn
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float TimeBetweenShots = 1.0f;

	//the particles spawned when this projectile explode
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ExplodeParticles = nullptr;

private:
	//when we collide with something
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//returns the output of a parabola used for the projectile's curve
	UFUNCTION()
	float GetCurveAdditive(float Input) const;
	
	//the initial distance between where this projectile spawns and its target
	UPROPERTY()
	float InitDistance;
	
	//where the projectile should head toward
	UPROPERTY()
	FVector TargetLocation;

	//the direction in which the projectile should travel
	UPROPERTY()
	FVector TargetDirection;

	//where this projectile was last frame
	UPROPERTY()
	FVector PreviousLocation;

	//the direction in which the projectile should curve
	UPROPERTY()
	FVector CurveDirection;

	//whether this projectile has reached the destination it was aiming for
	UPROPERTY()
	bool bTargetReached = false;

	//the character actor who spawned the projectile
	UPROPERTY()
	AActor* WizardOwner;
	
	
};
