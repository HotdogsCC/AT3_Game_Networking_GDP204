// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MainMenuWidget.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, WasSuccessful);


/**
 * 
 */
UCLASS()
class WIZARDTESTING_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//when the subsystem is created
    UMultiplayerSessionsSubsystem();

	//initialization; contains delagate bindings
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//deinitialization; currently unused
	virtual void Deinitialize() override;

	//starts up a server from a server name
	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	//searches for a server based on the server name
	UFUNCTION(BlueprintCallable)
	void JoinServer(FString ServerName);

	//called if the server is finished creating, possibly not successfully
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//called when a server is finished closing, possibly not successfully
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//called when a server is finished being found, possibly not successfully
	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	//called when a server is finished joining, possibly not successfu;;y
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//a reference to the online session interface
	IOnlineSessionPtr SessionInterface;

	//whether a session needs to created after the current once is destroyed
	bool bCreateServerAfterDestroy;

	//the name of the server to destroy
	FString DestroyServerName;

	//the name of the server to find
	FString ServerNameToFind;

	//the name of this session
	FName MySessionName;

	//reference to online session search
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//delegate called when server is created
	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate ServerCreateDel;

	//sets a reference to the main menu widget
	void AddMainMenuWidget(UMainMenuWidget* inWidget);

	//the main menu widget
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget = nullptr;
	
};
