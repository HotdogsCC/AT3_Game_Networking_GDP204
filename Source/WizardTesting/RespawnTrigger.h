// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RespawnTrigger.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDTESTING_API ARespawnTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	AActor* RespawnLocation = nullptr;

	
};
