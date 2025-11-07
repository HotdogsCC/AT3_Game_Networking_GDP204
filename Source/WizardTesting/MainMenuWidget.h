// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDTESTING_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//when the widget is constructed
	virtual void NativeConstruct() override;

	//displays an error message for an empty server name
	UFUNCTION(BlueprintImplementableEvent)
	void SetErrorMessage(const FText& ErrorMessage);

protected:
	//displays an error message if steam is not working
	UFUNCTION(BlueprintCallable)
	bool IsSteamWorking() const;
	
};
