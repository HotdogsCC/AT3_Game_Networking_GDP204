// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;
class UCanvasPanelSlot;
class AWizardCharacter;
/**
 * 
 */
UCLASS()
class WIZARDTESTING_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//when the widget is created
	virtual void NativeConstruct() override;

	//when the player can no longer fire
	UFUNCTION()
	void CannotFire();

	//when the player is able to fire
	UFUNCTION()
	void CanFire() const;

	//makes the hud bob up and down based on player speed
	UFUNCTION()
	void HeadBob(float PlayerSpeed);

	//updates scoreboard
	UFUNCTION()
	void SetDeaths(int32 ServerDeaths, int32 TheirDeaths);

protected:
	//the canvas containing the hands
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* HandCanvas = nullptr;

	//the image slot for the left hand
	UPROPERTY(meta=(BindWidget))
	UImage* LeftHand = nullptr;

	//the text for our deaths
	UPROPERTY(meta=(BindWidget))
	UTextBlock* YourDeathText = nullptr;

	//the text for their deaths
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TheirDeathText = nullptr;

	//the texture of the hand holding fire
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandWithFire;

	//the texture of the hand holding nothing
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandWithNoFire;

	//the texture of the hand casting the magic
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandCastingFire; 

private:
	//called shortly after CannotFire; makes hud transition from casting to holding nothing
	UFUNCTION()
	void FinishCasting() const;

	//reference to the player
	UPROPERTY()
	AWizardCharacter* Player = nullptr;

	//the canvas with the hands as a canvas slot; needed for transformation
	UPROPERTY()
	UCanvasPanelSlot* HandCanvasAsSlot = nullptr;

	//a handle for the delay between CannotFire() and FinishCasting()
	UPROPERTY()
	FTimerHandle FireCastingTimerHandle;
};
