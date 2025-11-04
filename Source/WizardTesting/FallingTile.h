// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingTile.generated.h"

UCLASS()
class WIZARDTESTING_API AFallingTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// sets which properties need to be replicated across the network
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//the mesh of this actor
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* StaticMesh = nullptr;

	//stores if the server or client see the truth behind the tile
	UPROPERTY(EditInstanceOnly, Category = "Tile Config", meta=(AllowPrivateAccess="true"))
	bool bServerCanSeeAnswer = true;

	//stores if this tile is safe to walk on
	UPROPERTY(ReplicatedUsing = SetupTile, EditInstanceOnly, Category = "Tile Config", meta=(AllowPrivateAccess="true"))
	bool bIsSafe = true;

	//stores if this tile flip every X seconds
	UPROPERTY(EditInstanceOnly, Category = "Tile Config|Flipping", meta = (AllowPrivateAccess = "true"))
	bool bShouldFlip = false;

	//the amount of seconds between flips
	UPROPERTY(EditInstanceOnly, Category = "Tile Config|Flipping", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenFlips = 3.0f;
	
	//regular material that the parkour player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* RegularMaterial = nullptr;

	//the safe material that the observer player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* SafeMaterial = nullptr;

	//the unsafe material that the observer player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* UnsafeMaterial = nullptr;

	//how long it has been since the last flip
	UPROPERTY()
	float ElapsedFlipTime = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//sets the material based on who is observing and its current safety
	UFUNCTION()
	void SetupTile();

	//attempts to change the material on the mesh
	void TrySetMaterial(UMaterial* InMaterial);

	//inverts its current safety status
	void Flip();

	//logic for flipping that occurs each tick
	void TickFlipTime(const float DeltaTime);

};
