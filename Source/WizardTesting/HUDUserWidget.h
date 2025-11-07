// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

class UImage;
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
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void CannotFire();

	UFUNCTION()
	void CanFire() const;

	UFUNCTION()
	void HeadBob(float PlayerSpeed);

protected:
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* HandCanvas = nullptr;

	UPROPERTY(meta=(BindWidget))
	UImage* LeftHand = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandWithFire;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandWithNoFire;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UTexture2D* HandCastingFire;

private:
	UFUNCTION()
	void FinishCasting() const;
	
	UPROPERTY()
	AWizardCharacter* Player = nullptr;

	UPROPERTY()
	UCanvasPanelSlot* HandCanvasAsSlot = nullptr;

	UPROPERTY()
	FTimerHandle FireCastingTimerHandle;
};
