// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColouredButton.generated.h"

class AColouredTile;

// Used by ColouredButton and ColouredTile to define their colours
UENUM()
enum class EColour : uint8
{
	RED,
	YELLOW,
	GREEN,
	BLUE
};

UCLASS()
class WIZARDTESTING_API AColouredButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AColouredButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The mesh used for the box of the button
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeStaticMesh;

	//The mesh used for the actual button component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonStaticMesh;

private:
	//called when an object collides with the button
	UFUNCTION()
	void OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//updates all coloured tiles to the new colour state
	UFUNCTION(NetMulticast, Reliable)
	void UpdateButtonSafety(EColour SafeColour);

	//the colour of this button
	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	EColour Colour = EColour::RED;

	//a reference to all coloured tiles in the scene
	UPROPERTY()
	TArray<AColouredTile*> ColouredTiles;

};
