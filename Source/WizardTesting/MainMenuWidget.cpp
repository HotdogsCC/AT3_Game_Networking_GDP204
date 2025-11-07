// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//give the subsystem a reference to the this
	GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>()->AddMainMenuWidget(this);
}


bool UMainMenuWidget::IsSteamWorking() const
{
	//grab the subsystem
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		//are we using steam?
		if (OnlineSubsystem->GetSubsystemName().ToString().Equals("STEAM"))
		{
			return true;
		}
	}

	return false;
}

