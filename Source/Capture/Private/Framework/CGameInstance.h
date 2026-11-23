// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "CGameInstance.generated.h"


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAuthLoginCompletedDelegate, bool /*bWasSuccessful*/, const FString& /*PlayerNickname*/, const FString& /*ErrorMessage*/);
/**
 * 
 */
UCLASS()
class UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:	
	virtual void Init() override;

	bool IsSessionServer() const;
	void PlayerJoined(const FUniqueNetIdRepl& UniqueId);
	void PlayerLeft(const FUniqueNetIdRepl& UniqueId);

private:
	void CreateSession();

	FOnlineSessionSettings GenerateSessionSetting(const FName& SessionName, const FString& SessionSearchID, int Port);
	void TerminateSessionServer();

	void EndSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnSessionCreated(FName SessionName, bool bWasSuccessful);

	UPROPERTY(EditDefaultsOnly, Category = "Session")
	float WaitPlayerJoinTimeoutDuration = 60.f;

	FTimerHandle WaitPlayerJoinTimeoutHandle;
	void WaitPlayerJoinTimeoutReached();

	TSet<FUniqueNetIdRepl> PlayerRecord;

	//-----------------------------------------//
	//               Game Client               // 
	//-----------------------------------------//
public:
	void RequestCreateNewSession();

	void TryFindAndJoinSession();

	FOnAuthLoginCompletedDelegate OnAuthLoginCompletedDelegate;
	bool IsLoggedIn() const;
	bool IsLoggingIn() const;

	void ClientAuthPortalLogin();
private:
	void ClientLoginCompleted(int LocalPlayerNumber, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorMsg);

	bool bIsLoggingIn = false;

	FString GetCoordinatorURL() const;

	UPROPERTY(EditDefaultsOnly, Category = "Coordinator")
	FString DEFAULT_COORDINATOR_URL = "127.0.0.1";

	FName COORDINATOR_URL_KEY = "COORDINATOR_URL";

	void SessionCreationRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully, FGuid SessionSearchId);

	void StartFindingAndJoinSession(FGuid SessionSearchID = FGuid{});

	FTimerHandle FindSessionTimerHandle;
	FTimerHandle FindSessionTimeoutTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Session Search")
	float SessionSearchInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Session Search")
	float SessionSearchTimeoutDuration = 30.f;

	void FindAndJoinSession(FGuid SessionSearchID);
	void StopSessionSearches();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void FindSessionCompleted(bool bWasSuccessful);

	void JoinSessionWithSearchResult(const FOnlineSessionSearchResult& SessionSearchResult);


	//-----------------------------------------//
	//          Online Interfaces              // 
	//-----------------------------------------//
private:
	IOnlineSessionPtr GetSessionInterface() const;
	IOnlineIdentityPtr GetIdentityInterface() const;

	//-----------------------------------------//
	//          Commandline Utilities          // 
	//-----------------------------------------//
private:
	FString GetSessionNameStr() const;
	FString GetSessionSearchIDStr() const;
	int GetSessionPort() const;


	FString GetCommandlineArgAsString(const FName& ParamName) const;
	int GetCommandlineArgAsInt(const FName& ParamName) const;

	UPROPERTY(EditDefaultsOnly, Category = "Commandline Args")
	FName SessionNameKey = "SESSION_NAME";

	UPROPERTY(EditDefaultsOnly, Category = "Commandline Args")
	FName SessionSearchIDKey = "SESSION_SEARCH_ID";

	UPROPERTY(EditDefaultsOnly, Category = "Commandline Args")
	FName SessionPortKey = "SESSION_PORT";

	FString ServerSessionName;
	int ServerSessionPort;

	//-----------------------------------------//
	//                 Map Loading             // 
	//-----------------------------------------//
private:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UWorld> GameLevel;

	void LoadLevelAndListen(TSoftObjectPtr<UWorld> Level);
};
