// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
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
	MySessionName = FName("CharlieCampbellAIE");
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
	//is the server name empty?
	if(ServerName.IsEmpty())
	{
		//is there a valid reference to the main menu UI?
		if (MainMenuWidget)
		{
			//update UI to display error
			FText ErrorMessage = FText::FromString("Server must contain a name");
			MainMenuWidget->SetErrorMessage(ErrorMessage);
		}
		
		//tell UI that it failed
		ServerCreateDel.Broadcast(false);
		return;
	}
	
	//is there already a session with this name?
	if(FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName))
	{
		//destroy it
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	//setup settings for this session
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	//grab the online subsystem
	if(IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		//are we actually online?
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

	//set the server name from what the player provided
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	//make the server
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::JoinServer(FString ServerName)
{
	//is the server name empty?
	if(ServerName.IsEmpty())
	{
		//check the main menu UI is valid
		if (MainMenuWidget)
		{
			//set the error message
			FText ErrorMessage = FText::FromString("Server must contain a name");
			MainMenuWidget->SetErrorMessage(ErrorMessage);
		}
		//tell UI that there was an error
		ServerCreateDel.Broadcast(false);
		return;
	}

	//make a struct for session search settings
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	//are we actually online?
	bool bIsLAN = false;
	if(Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL")
	{
		bIsLAN = true;
	}

	//set session search settings
	SessionSearch->bIsLanQuery = bIsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	//set the name of the server to search for
	ServerNameToFind = ServerName;

	//search for it
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//tell UI the status of the server creation
	ServerCreateDel.Broadcast(bWasSuccessful);

	//was it created?
	if(bWasSuccessful)
	{
		//load the game level
		GetWorld()->ServerTravel("/Game/TwoPlayerGame?listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	//was the server destroyed?
	if(bCreateServerAfterDestroy)
	{
		//create the server in the same name
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	//report to the UI the successfulness
	ServerCreateDel.Broadcast(bWasSuccessful);

	//did it fail?
	if(!bWasSuccessful)
	{
		//do nothing more
		return;
	}

	//is the server name empty?
	if(ServerNameToFind.IsEmpty())
	{
		//do nothing more
		return;
	}

	//grab an array of all servers that fit the search criteria
	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = nullptr;

	//are there any results?
	if(Results.Num() > 0)
	{
		//for each result
		for(FOnlineSessionSearchResult Result : Results)
		{
			//is the result actually valid?
			if(Result.IsValid())
			{
				//grab the server name
				FString ServerName = "Temp-no-name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				//is this the name we are trying to find?
				if(ServerName.Equals(ServerNameToFind))
				{
					//store this result as the correct one
					CorrectResult = &Result;
					break;
				}
			}
		}

		//was a correct result found?
		if(CorrectResult)
		{
			//join that server
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
			return;
		}
	}
	
	//tell the server it failed to join
	if (MainMenuWidget)
	{
		FText ErrorMessage = FText::FromString("Server was not found");
		MainMenuWidget->SetErrorMessage(ErrorMessage);
	}
	ServerCreateDel.Broadcast(false);
	
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//was the server joined successfully?
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		//try to get address 
		FString Address = "";
		if(SessionInterface->GetResolvedConnectString(MySessionName, Address))
		{
			//grab the player controller
			if(APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				//send the player to the server
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
				return;
			}
		}
	}
	
	//tell the UI it failed to join
	if (MainMenuWidget)
	{
		FText ErrorMessage = FText::FromString("Failed to connect to Server");
		MainMenuWidget->SetErrorMessage(ErrorMessage);
	}
	ServerCreateDel.Broadcast(false);
}

void UMultiplayerSessionsSubsystem::AddMainMenuWidget(UMainMenuWidget* inWidget)
{
	//set this reference to the main menu HUD
	MainMenuWidget = inWidget;
}
