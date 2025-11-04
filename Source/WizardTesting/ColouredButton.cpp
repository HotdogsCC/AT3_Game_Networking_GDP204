// Fill out your copyright notice in the Description page of Project Settings.


#include "ColouredButton.h"
#include "ColouredTile.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AColouredButton::AColouredButton()
{
	//create the cube mesh
	CubeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");
	//assign the cube as the root of the object
	CubeStaticMesh->SetupAttachment(RootComponent);
	//create the button mesh
	ButtonStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Button Mesh");
	//set the button as a child to the cube
	ButtonStaticMesh->SetupAttachment(CubeStaticMesh);
}

// Called when the game starts or when spawned
void AColouredButton::BeginPlay()
{
	Super::BeginPlay();

	//setup delegate for button collision eveents
	ButtonStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AColouredButton::OnButtonHit);

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

void AColouredButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//don't do anything if we aren't the server
	if (!HasAuthority())
	{
		return;
	}

	//tell the server to update their colours
	UpdateButtonSafety(Colour);
}

void AColouredButton::UpdateButtonSafety_Implementation(EColour SafeColour)
{
	//for every coloured tile
	for (AColouredTile* ColouredTile : ColouredTiles)
	{
		//is the tile the same colour as this button?
		if (ColouredTile->GetColour() == SafeColour)
		{
			//it is now safe
			ColouredTile->SetTileSafe();
		}
		else
		{
			//it is no longer safe
			ColouredTile->SetTileUnsafe();
		}
	}
}
