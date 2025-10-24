// Fill out your copyright notice in the Description page of Project Settings.


#include "JointButton.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AJointButton::AJointButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");
	CubeStaticMesh->SetupAttachment(RootComponent);
	ButtonStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Button Mesh");
	ButtonStaticMesh->SetupAttachment(CubeStaticMesh);

}

void AJointButton::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJointButton, bIsActive);
}


// Called when the game starts or when spawned
void AJointButton::BeginPlay()
{
	Super::BeginPlay();

	//setup button as off
	OnButtonOff();
	
	//bind the delegate for collision
	ButtonStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AJointButton::OnButtonHit);

	//look for all buttons in the scene
	TArray<AActor*> AllButtons;
	UGameplayStatics::GetAllActorsOfClass(this, AJointButton::StaticClass(), AllButtons);

	//loop through each one to find the button that isn't us
	for (AActor* PossibleButton : AllButtons)
	{
		//skip if this is us
		if (PossibleButton == this)
		{
			continue;
		}

		//otherwise, we're done!
		OtherButton = Cast<AJointButton>(PossibleButton);
		return;
	}
	
}

// Called every frame
void AJointButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//don't do anything if we are not the server
	if (!HasAuthority())
	{
		return;
	}

	TickButtonActive(DeltaTime);

}

void AJointButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//don't do anything if we are not the server
	if (!HasAuthority())
	{
		return;
	}

	OnButtonOn();
}

void AJointButton::TickButtonActive(float DeltaTime)
{
	//tick down the elapsed time of being active
	ElapsedActiveTime -= DeltaTime;

	//are we still active?
	if (ElapsedActiveTime < 0.0f)
	{
		OnButtonOff();
	}
}

bool AJointButton::GetIsActive() const
{
	return bIsActive;
}

void AJointButton::OnButtonOn()
{
	//reset countdown
	ElapsedActiveTime = ActiveTime;

	//enable us as active
	bIsActive = true;

	//check that we have a reference to the other button
	if (!OtherButton)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No reference to the other button");
		return;
	}

	//check the other button status
	if (OtherButton->GetIsActive())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Opened!");
	}

	//set material for the button
	SetButtonMaterial(bIsActive);
	
}

void AJointButton::OnButtonOff()
{
	//set countdown back to 0
	ElapsedActiveTime = 0.0f;
	
	//disbale us
	bIsActive = false;

	//change material for button
	SetButtonMaterial(bIsActive);
}

void AJointButton::SetButtonMaterial(bool bIsOn)
{
	if (bIsOn)
	{
		//check we have the material for turning on
		if (!OnMaterial)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No material set for On");
			return;
		}

		//set material
		ButtonStaticMesh->SetMaterial(0, OnMaterial);
	}

	else
	{
		//check we have the material for turning off
		if (!OffMaterial)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No material set for Off");
			return;
		}

		//set material
		ButtonStaticMesh->SetMaterial(0, OffMaterial);
	}
}

void AJointButton::OnRepIsActive()
{
	SetButtonMaterial(bIsActive);
}
