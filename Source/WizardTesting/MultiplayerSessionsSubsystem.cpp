// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

//shortcut for printing a message on the viewport
void PrintString(const FString& inputString)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, inputString);
	}
	
}

//construction
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	bCreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Project W Session Name");
}

//initialization
void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//grab the online subsystem
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		//grab the session interface 
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			//bind the delegates to the session interface

			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionsSubsystem::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionsSubsystem::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionsSubsystem::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		}
	}
}

//currently unused
void UMultiplayerSessionsSubsystem::Deinitialize()
{
	
}

//when the user creates a server
void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
	PrintString("Creating Server");

	if(ServerName.IsEmpty())
	{
		if (MainMenuWidget)
		{
			MainMenuWidget->CreatedServerWithNoName();
		}
		PrintString("Server name cannot be empty");
		ServerCreateDel.Broadcast(false);
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if(ExistingSession)
	{
		PrintString("session already exists, destroying it.");
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	
	if(IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if(SubsystemName.Equals("NULL"))
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
	}

	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::JoinServer(FString ServerName)
{
	PrintString("Joining Server");

	if(ServerName.IsEmpty())
	{
		PrintString("Server Name cannot be empty");
		ServerCreateDel.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = false;
	if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("On create session complete: %d"), bWasSuccessful));

	ServerCreateDel.Broadcast(bWasSuccessful);
	
	if(bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/Levels/TwoPlayerGame?listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	PrintString(FString::Printf(TEXT("On destroy session complete: %d"), bWasSuccessful));

	if(bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	ServerCreateDel.Broadcast(bWasSuccessful);
	if(!bWasSuccessful)
	{
		
		return;
	}

	if(ServerNameToFind.IsEmpty())
	{
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = nullptr;
	
	if(Results.Num() > 0)
	{
		PrintString("Successfully found some servers");

		for(FOnlineSessionSearchResult Result : Results)
		{
			if(Result.IsValid())
			{
				FString ServerName = "No-name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if(ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &Result;
					PrintString(ServerName);
					break;
				}
			}
		}

		if(CorrectResult)
		{
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}
		else
		{
			PrintString("Couldn't find server");
			ServerNameToFind = "";
			ServerCreateDel.Broadcast(false);
		}
	}
	else
	{
		PrintString("no servers found");
		ServerCreateDel.Broadcast(false);
	}
	
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		PrintString("Successfully joined session");

		FString Address = "";
		bool bSuccess = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if(bSuccess)
		{
			PrintString("address worked");
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(PlayerController)
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("getresolvedconnectstring returned false");
		}
	}
	else
	{
		PrintString("OnJoinSessionComplete failed");
	}
}

void UMultiplayerSessionsSubsystem::AddMainMenuWidget(UMainMenuWidget* inWidget)
{
	MainMenuWidget = inWidget;
}
