// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTile.h"

// Sets default values
AFallingTile::AFallingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup default static mesh for actor
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AFallingTile::BeginPlay()
{
	Super::BeginPlay();

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
