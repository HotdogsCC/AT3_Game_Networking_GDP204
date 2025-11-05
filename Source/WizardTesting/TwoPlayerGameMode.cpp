// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoPlayerGameMode.h"

#include "ColouredButton.h"
#include "ColouredTile.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ATwoPlayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	ActiveColour1 = EColour::RED;
	ActiveColour2 = EColour::YELLOW;

	//find all coloured tiles
	TArray<AActor*> TempColouredTiles;
	UGameplayStatics::GetAllActorsOfClass(this, AColouredTile::StaticClass(), TempColouredTiles);

	//load them into the array
	for (AActor* Actor : TempColouredTiles)
	{
		//try turn into a coloured tile
		if (AColouredTile* ColouredTile = Cast<AColouredTile>(Actor))
		{
			//add the coloured tile to our perm reference
			ColouredTiles.Add(ColouredTile);
		}
	}

	//clear the temp array of actors
	TempColouredTiles.Empty();
}


AActor* ATwoPlayerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	Super::ChoosePlayerStart_Implementation(Player);

	//find all the player starts
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	//get the next unused player start
	AActor* ThisPlayerStart = PlayerStarts[PlayerStartIndex];
	checkf(ThisPlayerStart, TEXT("Failed to find Player Start at index %d"), PlayerStartIndex);

	//increment the player start index
	PlayerStartIndex++;

	//return where the player should start
	return ThisPlayerStart; 
}

void ATwoPlayerGameMode::ActivateColour(EColour NewColour)
{
	//is this colour already active?
	if (NewColour == ActiveColour1 || NewColour == ActiveColour2)
	{
		//do nothing more
		return;
	}

	EColour OldColour;

	//which colour is the oldest
	if (isColour1ReplacedNext)
	{
		OldColour = ActiveColour1;
		ActiveColour1 = NewColour;
	}
	else
	{
		OldColour = ActiveColour2;
		ActiveColour2 = NewColour;
	}

	isColour1ReplacedNext = !isColour1ReplacedNext;

	//for every coloured tile
	for (AColouredTile* ColouredTile : ColouredTiles)
	{
		//is the tile the same colour as this button?
		if (ColouredTile->GetColour() == NewColour)
		{
			//it is now safe
			ColouredTile->SetTileSafe();
		}
		else if (ColouredTile->GetColour() == OldColour)
		{
			//it is no longer safe
			ColouredTile->SetTileUnsafe();
		}
	}
}
