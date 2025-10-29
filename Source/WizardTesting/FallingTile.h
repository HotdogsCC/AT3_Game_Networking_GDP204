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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//the mesh of this actor
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* StaticMesh = nullptr;

	//can server or client see the truth behind the tile
	UPROPERTY(EditInstanceOnly, Category = "Tile Config", meta=(AllowPrivateAccess="true"))
	bool bServerCanSeeAnswer = true;

	//will this tile remain stationary
	UPROPERTY(ReplicatedUsing = SetupTile, EditInstanceOnly, Category = "Tile Config", meta=(AllowPrivateAccess="true"))
	bool bIsSafe = true;

	//will this tile flip every X seconds
	UPROPERTY(EditInstanceOnly, Category = "Tile Config|Flipping", meta = (AllowPrivateAccess = "true"))
	bool bShouldFlip = false;

	//the amount of seconds between flips
	UPROPERTY(EditInstanceOnly, Category = "Tile Config|Flipping", meta = (AllowPrivateAccess = "true"))
	float TimeBetweenFlips = 3.0f;

	float ElapsedFlipTime = 0.0f;

	//regular material that the targeted player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* RegularMaterial = nullptr;

	//regular material that the targeted player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* SafeMaterial = nullptr;

	//regular material that the targeted player will see
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(AllowPrivateAccess="true"))
	UMaterial* UnsafeMaterial = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void SetupTile();
	void TrySetMaterial(UMaterial* InMaterial);
	void Flip();


	void TickFlipTime(const float DeltaTime);

};
