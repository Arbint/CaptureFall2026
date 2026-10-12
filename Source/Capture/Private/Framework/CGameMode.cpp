// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/CGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

APlayerController* ACGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewController = Super::SpawnPlayerController(InRemoteRole, Options);
	IGenericTeamAgentInterface* ControllerTeamInterface = Cast<IGenericTeamAgentInterface>(NewController);
	FGenericTeamId TeamId = GetTeamIdForPlayer(NewController);
	if (ControllerTeamInterface)
	{
		ControllerTeamInterface->SetGenericTeamId(TeamId);
	}

	NewController->StartSpot = FindNextStartSpotForTeam(TeamId);

	return NewController;
}

FGenericTeamId ACGameMode::GetTeamIdForPlayer(const APlayerController* PlayerController)
{
	static int PlayerCount = 0;
	++PlayerCount;

	return FGenericTeamId(PlayerCount%2);
}

AActor* ACGameMode::FindNextStartSpotForTeam(const FGenericTeamId& TeamId)
{
	const FName* StartSpotTag = TeamPlayerStartTagMap.Find(TeamId);
	if (!StartSpotTag)
	{
		return nullptr;
	}

	//TArray<int> testInt{1,2,3,4};
	//for (int i : testInt)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), i)
	//}

	//for (auto it = testInt.CreateIterator(); it; ++it)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), *it)
	//}

	if (UWorld* World = GetWorld())
	{
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			if (It->PlayerStartTag == *StartSpotTag)
			{
				It->PlayerStartTag = FName("Taken");
				return *It;
			}
		}
	}

	return nullptr;
}
