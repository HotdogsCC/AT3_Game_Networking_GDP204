// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "OnlineSubsystem.h"

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
