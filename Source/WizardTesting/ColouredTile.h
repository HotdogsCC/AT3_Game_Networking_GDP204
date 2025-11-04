// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColouredButton.h"
#include "ColouredTile.generated.h"

UCLASS()
class WIZARDTESTING_API AColouredTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColouredTile();

	//returns the colour of this tile
	EColour GetColour() const;

	//allows players to walk on this tile
	void SetTileSafe();

	//disables collision on the tile, preventing players from walking
	void SetTileUnsafe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//the mesh of this actor
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;

	//the material assigned to the mesh when it is safe to walk on
	UPROPERTY(EditDefaultsOnly, Category="Materials", meta = (AllowPrivateAccess = "true"))
	UMaterial* SafeMat = nullptr;

	//the material assigned to the mesh when it is not safe to walk on
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterial* UnsafeMat = nullptr;

private:
	//change the material and collision settings based on the new saftey status
	UFUNCTION()
	void ApplyNewSafety();

	//the colour of this tile
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	EColour Colour = EColour::RED;

	//whether players are able to walk on the tile or not
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	bool bIsSafe = false;
};
