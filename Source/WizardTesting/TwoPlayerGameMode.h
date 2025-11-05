// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoPlayerGameMode.generated.h"

class AColouredTile;
enum class EColour : uint8;
/**
 * 
 */
UCLASS()
class WIZARDTESTING_API ATwoPlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//returns the location a player should start at
	UFUNCTION()
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//activates one colour, and deactivates the oldest colour
	UFUNCTION()
	void ActivateColour(EColour NewColour); 

private:
	//a reference to all coloured tiles in the scene
	UPROPERTY()
	TArray<AColouredTile*> ColouredTiles;

	//the index of the player start to query from
	int32 PlayerStartIndex = 0;

	EColour ActiveColour1;
	EColour ActiveColour2;
	bool isColour1ReplacedNext = true;
};
