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

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//the mesh of this actor
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Materials", meta = (AllowPrivateAccess = "true"))
	UMaterial* RedMat = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterial* TransRedMat = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EColour GetColour() const;

	void SetTileSafe();
	void SetTileUnsafe();

private:
	UFUNCTION()
	void ApplyNewSafety();

	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	EColour Colour = EColour::RED;

	UPROPERTY(ReplicatedUsing = ApplyNewSafety, EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	bool bIsSafe = false;
};
