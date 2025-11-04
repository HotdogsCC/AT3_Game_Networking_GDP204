// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTile.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AFallingTile::AFallingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup default static mesh for actor
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMesh;
}

void AFallingTile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//replicate this tiles safety status over the network
	DOREPLIFETIME(AFallingTile, bIsSafe);
}


// Called when the game starts or when spawned
void AFallingTile::BeginPlay()
{
	Super::BeginPlay();

	//set up the tile's material and collision based on its current safety
	SetupTile();
}

void AFallingTile::SetupTile()
{
	//is this tile safe?
	if (bIsSafe)
	{
		//enable collision
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		//disable collision
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//can this player see the answer?
	if (HasAuthority() == bServerCanSeeAnswer)
	{
		//is this a safe tile?
		if (bIsSafe)
		{
			//apply safe material
			TrySetMaterial(SafeMaterial);
		}
		else
		{
			//apply unsafe material
			TrySetMaterial(UnsafeMaterial);
		}
	}
	else
	{
		//apply regular material
		TrySetMaterial(RegularMaterial);
	}
}

// Called every frame
void AFallingTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//are we the client?
	if (!HasAuthority())
	{
		//dont do anything more
		return;
	}

	//are we a flipping tile?
	if (bShouldFlip)
	{
		//tick flip logic
		TickFlipTime(DeltaTime);
	}
		
}

void AFallingTile::TickFlipTime(const float DeltaTime)
{
	//increase the time since the last flip
	ElapsedFlipTime += DeltaTime;

	//has it been long enough since the last flip?
	if (ElapsedFlipTime >= TimeBetweenFlips)
	{
		//reset flip timer
		ElapsedFlipTime = 0;

		//flip the safety status
		Flip();
	}
}


void AFallingTile::TrySetMaterial(UMaterial* InMaterial)
{
	//is our material invalid?
	if (!InMaterial)
	{
		//log the error and do nothing more
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Material not set in Falling Tile");
		return;
	}

	//apply the material
	StaticMesh->SetMaterial(0, InMaterial);
}

void AFallingTile::Flip()
{
	//flip the safety status
	bIsSafe = !bIsSafe;

	//apply new material and collision settings
	SetupTile();

}
