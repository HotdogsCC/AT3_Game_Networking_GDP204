// Fill out your copyright notice in the Description page of Project Settings.


#include "JointButton.h"

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

	ButtonStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AJointButton::OnButtonHit);
	
}

// Called every frame
void AJointButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJointButton::OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("touched"));
}

