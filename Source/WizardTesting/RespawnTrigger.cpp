// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnTrigger.h"

#include "GameFramework/Character.h"

void ARespawnTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void ARespawnTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    //check we are a server
    if (!HasAuthority())
        return;
    
    //is it a character?
    if (ACharacter* Player = Cast<ACharacter>(OtherActor))
    {
        if (!RespawnLocation)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No respawn location set");
            return;
        }
        
        Player->SetActorLocation(RespawnLocation->GetActorLocation());
    }
}
