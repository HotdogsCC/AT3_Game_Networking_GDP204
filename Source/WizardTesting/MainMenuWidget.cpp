// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

void UMainMenuWidget::NativeConstruct()
{
	GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>()->AddMainMenuWidget(this);
}


bool UMainMenuWidget::IsSteamWorking() const
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if (SubsystemName.Equals("STEAM"))
		{
			return true;
		}
	}

	return false;
}

