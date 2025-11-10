// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"
#include "WizardCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//get a reference to the owning player
	Player = Cast<AWizardCharacter>(GetOwningPlayer()->GetPawn());
	checkf(Player, TEXT("Failed to get reference to owning Player"));

	//cast the hand canvas to a slot
	HandCanvasAsSlot = Cast<UCanvasPanelSlot>(HandCanvas->Slot);
	checkf(HandCanvasAsSlot, TEXT("Failed to cast HandCanvasSlot"));
}

void UHUDUserWidget::HeadBob(float PlayerSpeed)
{
	//get time in seconds
	const float Time = UGameplayStatics::GetTimeSeconds(this);

	//clamp speed (so it is always bobbing)
	constexpr float MinPlayerSpeed = 120.0f;
	constexpr float MaxPlayerSpeed = 100000.0f;
	const float ClampedPlayerSpeed = FMath::Clamp(PlayerSpeed, MinPlayerSpeed, MaxPlayerSpeed);

	//set the speed and size of the bob
	constexpr float HeadBobSpeed = 10.0f;
	constexpr float HeadBobSizeScale = 0.02f;

	//calculate sin for this frame
	float Offset = FMath::Sin(HeadBobSpeed * Time);

	//transform sin between 0 and 1
	Offset += 1.0f;
	Offset /= 2.0f;

	//transform by size and speed
	Offset *= ClampedPlayerSpeed;
	Offset *= HeadBobSizeScale;

	//apply transformation
	FMargin Margin;
	Margin.Bottom = -Offset;
	HandCanvasAsSlot->SetOffsets(Margin);
	
}

void UHUDUserWidget::CanFire() const
{
	//make the ui show an image of a hand with fire
	LeftHand->SetBrushFromTexture(HandWithFire);
}

void UHUDUserWidget::CannotFire()
{
	//make the ui show the hand casting the fire
	LeftHand->SetBrushFromTexture(HandCastingFire);

	//make it show an empty hand in half a second
	GetWorld()->GetTimerManager().SetTimer(FireCastingTimerHandle, this,
		&UHUDUserWidget::FinishCasting, 0.5f, false);
}

void UHUDUserWidget::SetDeaths(int32 OurDeaths, int32 TheirDeaths)
{
	FString YourDeathString = "Your Deaths: ";
	YourDeathString.AppendInt(OurDeaths);

	FString TheirDeathString = "Their Deaths: ";
	TheirDeathString.AppendInt(TheirDeaths);
	
	YourDeathText->SetText(FText::FromString(YourDeathString));
	TheirDeathText->SetText(FText::FromString(TheirDeathString));
}




void UHUDUserWidget::FinishCasting() const
{
	//make the ui show an empty hand
	LeftHand->SetBrushFromTexture(HandWithNoFire);
}