// Fill out your copyright notice in the Description page of Project Settings.


#include "ColouredTile.h"

#include <Net/UnrealNetwork.h>

// Sets default values
AColouredTile::AColouredTile()
{
	//setup default static mesh for actor
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMesh;

}


// Called when the game starts or when spawned
void AColouredTile::BeginPlay()
{
	Super::BeginPlay();

	//setup this tile based on its current saftey status
	ApplyNewSafety();
	
}

void AColouredTile::ApplyNewSafety()
{
	//is this tile safe?
	if (bIsSafe)
	{
		//enable collision
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//set the material to be the safe material
		StaticMesh->SetMaterial(0, SafeMat);
	}
	else
	{
		//disable collision
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//set the material to be the unsafe material
		StaticMesh->SetMaterial(0, UnsafeMat);
	}
}

EColour AColouredTile::GetColour() const
{
	//return the colour of the tile
	return Colour;
}

void AColouredTile::SetTileSafe()
{
	//set this tile as safe
	bIsSafe = true;
	//apply
	ApplyNewSafety();
}

void AColouredTile::SetTileUnsafe()
{
	//set this tile as unsafe
	bIsSafe = false;
	//apply
	ApplyNewSafety();
}
