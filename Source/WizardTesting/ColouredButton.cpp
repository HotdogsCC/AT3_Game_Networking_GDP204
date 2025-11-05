// Fill out your copyright notice in the Description page of Project Settings.


#include "ColouredButton.h"
#include <Kismet/GameplayStatics.h>

#include "TwoPlayerGameMode.h"

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

	//setup delegate for button collision events
	ButtonStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AColouredButton::OnButtonHit);

	
	
}

void AColouredButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//don't do anything if we aren't the server
	if (!HasAuthority())
	{
		return;
	}

	//tell the server to update their colours
	Cast<ATwoPlayerGameMode>(UGameplayStatics::GetGameMode(this))->ActivateColour(Colour);
}


