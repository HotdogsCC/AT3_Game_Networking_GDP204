// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"

#include "Kismet/GameplayStatics.h"

void UHUDUserWidget::HeadBob(float PlayerSpeed)
{
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	constexpr float MinPlayerSpeed = 120.0f;
	constexpr float MaxPlayerSpeed = 100000.0f;
	const float ClampedPlayerSpeed = FMath::Clamp(PlayerSpeed, MinPlayerSpeed, MaxPlayerSpeed);

	constexpr float HeadBobSpeed = 10.0f;
	constexpr float HeadBobSizeScale = 0.02f;

	float Offset = FMath::Sin(HeadBobSpeed * DeltaTime);

	Offset += 1.0f;
	Offset /= 2.0f;

	Offset *= ClampedPlayerSpeed;
	Offset *= HeadBobSizeScale;

	//SET POS OF HAND CANVAS SLOT
	
}