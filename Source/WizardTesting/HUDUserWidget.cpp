// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"
#include "WizardCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<AWizardCharacter>(GetOwningPlayer()->GetPawn());
	checkf(Player, TEXT("Failed to get reference to owning Player"));

	HandCanvasAsSlot = Cast<UCanvasPanelSlot>(HandCanvas->Slot);
	checkf(HandCanvasAsSlot, TEXT("Failed to cast HandCanvasSlot"));
}

void UHUDUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	HeadBob(Player->GetPlayerSpeed());
}


void UHUDUserWidget::HeadBob(float PlayerSpeed)
{
	const float Time = UGameplayStatics::GetTimeSeconds(this);
	
	constexpr float MinPlayerSpeed = 120.0f;
	constexpr float MaxPlayerSpeed = 100000.0f;
	const float ClampedPlayerSpeed = FMath::Clamp(PlayerSpeed, MinPlayerSpeed, MaxPlayerSpeed);

	constexpr float HeadBobSpeed = 10.0f;
	constexpr float HeadBobSizeScale = 0.02f;

	float Offset = FMath::Sin(HeadBobSpeed * Time);

	Offset += 1.0f;
	Offset /= 2.0f;

	Offset *= ClampedPlayerSpeed;
	Offset *= HeadBobSizeScale;
	
	HandCanvasAsSlot->SetPosition(FVector2D(0.0f, Offset));
	
}

void UHUDUserWidget::CanFire() const
{
	LeftHand->SetBrushFromTexture(HandWithFire);
}

void UHUDUserWidget::CannotFire()
{
	LeftHand->SetBrushFromTexture(HandCastingFire);

	GetWorld()->GetTimerManager().SetTimer(FireCastingTimerHandle, this,
		&UHUDUserWidget::FinishCasting, 0.5f, false);
}

void UHUDUserWidget::FinishCasting() const
{
	LeftHand->SetBrushFromTexture(HandWithNoFire);
}

