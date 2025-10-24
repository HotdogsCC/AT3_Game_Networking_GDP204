// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningDoor.h"

// Sets default values
AOpeningDoor::AOpeningDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOpeningDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOpeningDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpening)
	{
		//move the door
		FVector NewLocation = GetActorLocation();
		NewLocation.Z -= DeltaTime * Speed;
		SetActorLocation(NewLocation);

		//increment despawn time
		ElapsedDespawnTime -= DeltaTime;
		if (ElapsedDespawnTime <= 0.0f)
		{
			Destroy();
		}
	}

}

void AOpeningDoor::OpenDoor()
{
	bIsOpening = true;

	ElapsedDespawnTime = DespawnTime;
}

