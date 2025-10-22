// Fill out your copyright notice in the Description page of Project Settings.


#include "JointButton.h"

#include "Kismet/GameplayStatics.h"

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

// Called when the game starts or when spawned
void AJointButton::BeginPlay()
{
	Super::BeginPlay();

	//bind the delegate for collision
	ButtonStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AJointButton::OnButtonHit);

	//look for the other button
	TArray<AActor*> AllButtons;
	TSubclassOf<AJointButton> JointButtonClass;
	UGameplayStatics::GetAllActorsOfClass(this, JointButtonClass, AllButtons);
	
}

// Called every frame
void AJointButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJointButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//dont do anything if we are not the server
	if (!HasAuthority())
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "touched");
	UE_LOG(LogTemp, Display, TEXT("touched"));
}

