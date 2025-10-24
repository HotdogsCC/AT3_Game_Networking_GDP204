// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpeningDoor.generated.h"

UCLASS()
class WIZARDTESTING_API AOpeningDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOpeningDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Speed = 100.0f;

	UPROPERTY(EditAnywhere)
	float DespawnTime = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenDoor();

	

private:
	UPROPERTY()
	bool bIsOpening = false;

	UPROPERTY()
	float ElapsedDespawnTime = 0.0f;

};
