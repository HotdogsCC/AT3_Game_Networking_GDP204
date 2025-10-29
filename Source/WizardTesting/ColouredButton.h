// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColouredButton.generated.h"

class AColouredTile;
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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeStaticMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonStaticMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	EColour Colour = EColour::RED;

	UPROPERTY()
	TArray<AColouredTile*> ColouredTiles;

};
