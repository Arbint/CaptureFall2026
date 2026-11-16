// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameInstance.h"

bool UCGameInstance::IsLoggedIn() const
{
	IOnlineIdentityPtr IdentityInterface = GetIdentityInterface();
	if (IdentityInterface)
	{
		return IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
	}

	return false;
}

void UCGameInstance::ClientAuthPortalLogin()
{
	IOnlineIdentityPtr IdentityInterface = GetIdentityInterface();
	if (IdentityInterface)
	{
		IdentityInterface->OnLoginCompleteDelegates->RemoveAll(this);
		IdentityInterface->OnLoginCompleteDelegates->AddUObject(this, &UCGameInstance::ClientLoginCompleted);
		if (!IdentityInterface->Login(0, FOnlineAccountCredentials("accountportal", "", "")))
		{
			UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
			UE_LOG(LogTemp, Warning, TEXT("Login in failed right away"))
			UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
		UE_LOG(LogTemp, Warning, TEXT("Login failed due to not finding identity interface"))
		UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
	}
}

void UCGameInstance::ClientLoginCompleted(int LocalPlayerNumber, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorMsg)
{

	IOnlineIdentityPtr IdentityInterface = GetIdentityInterface();
	if (IdentityInterface)
	{
		IdentityInterface->OnLoginCompleteDelegates->RemoveAll(this);
		if (bWasSuccessful)
		{
			FString PlayerNickname = IdentityInterface->GetPlayerNickname(UserId);
			UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
			UE_LOG(LogTemp, Warning, TEXT("Logged in successful as: %s"), *(PlayerNickname))
			UE_LOG(LogTemp, Warning, TEXT("---------------------------"))
			OnAuthLoginCompletedDelegate.Broadcast(bWasSuccessful, PlayerNickname, ErrorMsg);
			return;
		}
	}

	OnAuthLoginCompletedDelegate.Broadcast(bWasSuccessful, "", ErrorMsg);
}


void UCGameInstance::Init()
{
	Super::Init();
	if (GetWorld()->IsEditorWorld())
		return;

	if (IsSessionServer())
	{
		CreateSession();
	}
}

bool UCGameInstance::IsSessionServer() const
{
	return GetWorld() && GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer;
}

void UCGameInstance::PlayerJoined(const FUniqueNetIdRepl& UniqueId)
{
	if (WaitPlayerJoinTimeoutHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitPlayerJoinTimeoutHandle);
	}
	PlayerRecord.Add(UniqueId);
}

void UCGameInstance::PlayerLeft(const FUniqueNetIdRepl& UniqueId)
{
	PlayerRecord.Remove(UniqueId);
	if (PlayerRecord.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("----All Player Left the Session, Terminating"))
		TerminateSessionServer();
	}
}

void UCGameInstance::CreateSession()
{
	if (IOnlineSessionPtr SessionPtr = GetSessionInterface())
	{
		UE_LOG(LogTemp, Warning, TEXT("------------Server Creating Session!-----------"))
		ServerSessionName = GetSessionNameStr();
		FString ServerSessionSearchID = GetSessionSearchIDStr();
		ServerSessionPort = GetSessionPort();

		UE_LOG(LogTemp, Warning, TEXT("#### Creating Session with Name: %s, Id: %s, Port: %d"),
			*ServerSessionName, *ServerSessionSearchID, ServerSessionPort)

		SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
		SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnSessionCreated);
		if(!SessionPtr->CreateSession(0, FName(ServerSessionName), GenerateSessionSetting(FName(ServerSessionName), ServerSessionSearchID, ServerSessionPort)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Session Creation failed rightaway!"))
			SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
			TerminateSessionServer();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find session ptr, terminating"))
		TerminateSessionServer();
	}
}

FOnlineSessionSettings UCGameInstance::GenerateSessionSetting(const FName& SessionName, const FString& SessionSearchID, int Port)
{
	FOnlineSessionSettings OnlineSessionSettings{};
	OnlineSessionSettings.bIsLANMatch = false;
	OnlineSessionSettings.NumPrivateConnections = 10;
	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionSettings.bUsesPresence = false;
	OnlineSessionSettings.bAllowJoinViaPresence = false;
	OnlineSessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	OnlineSessionSettings.bAllowInvites = true;
	OnlineSessionSettings.bUseLobbiesIfAvailable = false;
	OnlineSessionSettings.bUseLobbiesVoiceChatIfAvailable = false;
	OnlineSessionSettings.bUsesStats = true;

	OnlineSessionSettings.Set(SessionNameKey, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OnlineSessionSettings.Set(SessionSearchIDKey, SessionSearchID, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	OnlineSessionSettings.Set(SessionPortKey, Port, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	return OnlineSessionSettings;
}

void UCGameInstance::TerminateSessionServer()
{
	if (IOnlineSessionPtr OnlineSessionPtr = GetSessionInterface())
	{
		OnlineSessionPtr->OnEndSessionCompleteDelegates.RemoveAll(this);
		OnlineSessionPtr->OnEndSessionCompleteDelegates.AddUObject(this, &UCGameInstance::EndSessionCompleted);
		if (!OnlineSessionPtr->EndSession(FName(ServerSessionName)))
		{
			FGenericPlatformMisc::RequestExit(false);
		}
	}
	else
	{
		FGenericPlatformMisc::RequestExit(false);
	}
}

void UCGameInstance::EndSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	FGenericPlatformMisc::RequestExit(false);
}

void UCGameInstance::OnSessionCreated(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s created successfully!"), *SessionName.ToString())
		LoadLevelAndListen(GameLevel);
		GetWorld()->GetTimerManager().SetTimer(WaitPlayerJoinTimeoutHandle, this, &UCGameInstance::WaitPlayerJoinTimeoutReached, WaitPlayerJoinTimeoutDuration);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s creation failed!"), *SessionName.ToString())
		TerminateSessionServer();
	}

	IOnlineSessionPtr SessionPtr = GetSessionInterface();
	if (SessionPtr)
	{
		SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
	}
}

void UCGameInstance::WaitPlayerJoinTimeoutReached()
{
	UE_LOG(LogTemp, Warning, TEXT("------------Session Server Shutdown after %f second without player joining---------"), WaitPlayerJoinTimeoutDuration)
	TerminateSessionServer();
}

IOnlineSessionPtr UCGameInstance::GetSessionInterface() const
{
	IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
	if (OnlineSubSystem)
	{
		return OnlineSubSystem->GetSessionInterface();
	}

	return nullptr;
}

IOnlineIdentityPtr UCGameInstance::GetIdentityInterface() const
{
	IOnlineSubsystem* OnlineSubSystem = IOnlineSubsystem::Get();
	if (OnlineSubSystem)
	{
		return OnlineSubSystem->GetIdentityInterface();
	}

	return nullptr;
}

FString UCGameInstance::GetSessionNameStr() const
{
	return GetCommandlineArgAsString(SessionNameKey);
}

FString UCGameInstance::GetSessionSearchIDStr() const
{
	return GetCommandlineArgAsString(SessionSearchIDKey);
}

int UCGameInstance::GetSessionPort() const
{
	return GetCommandlineArgAsInt(SessionPortKey);
}

FString UCGameInstance::GetCommandlineArgAsString(const FName& ParamName) const
{
	FString OutVal = "";
	FString CommandlineArg = FString::Printf(TEXT("%s="), *(ParamName.ToString()));
	if (!FParse::Value(FCommandLine::Get(), *CommandlineArg, OutVal))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find command line argument: %s"), *(ParamName.ToString()))
	}

	return OutVal;
}

int UCGameInstance::GetCommandlineArgAsInt(const FName& ParamName) const
{
	int OutVal = 0;
	FString CommandlineArg = FString::Printf(TEXT("%s="), *(ParamName.ToString()));
	if (!FParse::Value(FCommandLine::Get(), *CommandlineArg, OutVal))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find command line argument: %s"), *(ParamName.ToString()))
	}

	return OutVal;
}

void UCGameInstance::LoadLevelAndListen(TSoftObjectPtr<UWorld> Level)
{
	const FName LevelURL = FName(*FPackageName::ObjectPathToPackageName(Level.ToString()));
	if (LevelURL != "")
	{
		FString TravelStr = FString::Printf(TEXT("%s?listen?port=%d"), *LevelURL.ToString(), ServerSessionPort);
		UE_LOG(LogTemp, Warning, TEXT("Server traveling to: %s"), *(TravelStr))
		GetWorld()->ServerTravel(TravelStr);
	}
}
