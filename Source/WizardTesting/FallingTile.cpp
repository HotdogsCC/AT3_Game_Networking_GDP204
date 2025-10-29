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

	DOREPLIFETIME(AFallingTile, bIsSafe);
}


// Called when the game starts or when spawned
void AFallingTile::BeginPlay()
{
	Super::BeginPlay();

	SetupTile();
}

void AFallingTile::SetupTile()
{
	if (bIsSafe)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//can this player see the truth?
	if (HasAuthority() == bServerCanSeeAnswer)
	{
		//is this a safe tile?
		if (bIsSafe)
		{
			TrySetMaterial(SafeMaterial);
		}
		else
		{
			TrySetMaterial(UnsafeMaterial);
		}
	}
	else
	{
		TrySetMaterial(RegularMaterial);
	}
}

// Called every frame
void AFallingTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//dont do anything if we arent the server
	if (!HasAuthority())
	{
		return;
	}

	if (bShouldFlip)
	{
		TickFlipTime(DeltaTime);
	}
		
}

void AFallingTile::TickFlipTime(const float DeltaTime)
{
	ElapsedFlipTime += DeltaTime;

	if (ElapsedFlipTime >= TimeBetweenFlips)
	{
		ElapsedFlipTime = 0;
		Flip();
	}
}


void AFallingTile::TrySetMaterial(UMaterial* InMaterial)
{
	if (!InMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Material not set in Falling Tile");
		return;
	}

	StaticMesh->SetMaterial(0, InMaterial);
}

void AFallingTile::Flip()
{
	bIsSafe = !bIsSafe;

	SetupTile();

}
