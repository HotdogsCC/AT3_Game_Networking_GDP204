// Fill out your copyright notice in the Description page of Project Settings.


#include "ColouredTile.h"

#include <Net/UnrealNetwork.h>

// Sets default values
AColouredTile::AColouredTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup default static mesh for actor
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMesh;

}


// Called when the game starts or when spawned
void AColouredTile::BeginPlay()
{
	Super::BeginPlay();

	ApplyNewSafety();
	
}

// Called every frame
void AColouredTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColouredTile::ApplyNewSafety()
{
	if (bIsSafe)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMesh->SetMaterial(0, SafeMat);
	}
	else
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetMaterial(0, UnsafeMat);
	}
}

EColour AColouredTile::GetColour() const
{
	return Colour;
}

void AColouredTile::SetTileSafe()
{
	bIsSafe = true;
	ApplyNewSafety();
}

void AColouredTile::SetTileUnsafe()
{
	bIsSafe = false;
	ApplyNewSafety();
}
