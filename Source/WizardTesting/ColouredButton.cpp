// Fill out your copyright notice in the Description page of Project Settings.


#include "ColouredButton.h"
#include "ColouredTile.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AColouredButton::AColouredButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");
	CubeStaticMesh->SetupAttachment(RootComponent);
	ButtonStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Button Mesh");
	ButtonStaticMesh->SetupAttachment(CubeStaticMesh);

}

// Called when the game starts or when spawned
void AColouredButton::BeginPlay()
{
	Super::BeginPlay();

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
			ColouredTiles.Add(ColouredTile);
		}
	}

	TempColouredTiles.Empty();
	
}

// Called every frame
void AColouredButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColouredButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//dont do anything if we arent the server
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
