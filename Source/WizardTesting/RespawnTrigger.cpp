// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnTrigger.h"

#include "TwoPlayerGameMode.h"
#include "WizardCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
    if (AWizardCharacter* Player = Cast<AWizardCharacter>(OtherActor))
    {
        if (!RespawnLocation)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No respawn location set");
            return;
        }
        
        Player->SetActorLocation(RespawnLocation->GetActorLocation());

        //is this the server player?
        if (Player == UGameplayStatics::GetPlayerCharacter(this, 0))
        {
            Cast<ATwoPlayerGameMode>(UGameplayStatics::GetGameMode(this))->ServerPlayerDied();
        }
        else
        {
            Cast<ATwoPlayerGameMode>(UGameplayStatics::GetGameMode(this))->ClientPlayerDied();
        }
    }
}
