// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MatchMakingWidget.h"
#include "Framework/CGameInstance.h"
#include "Components/Button.h"

void UMatchMakingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CGameInstance = GetGameInstance<UCGameInstance>();

	CreateNewSessionBtn->OnClicked.AddDynamic(this, &UMatchMakingWidget::CreateNewSessionBtnClicked);
	FindAndJoinSessionBtn->OnClicked.AddDynamic(this, &UMatchMakingWidget::FindAndJoinSesionBtnClicked);
}

void UMatchMakingWidget::CreateNewSessionBtnClicked()
{
	if (CGameInstance)
	{
		CGameInstance->RequestCreateNewSession();
	}
}

void UMatchMakingWidget::FindAndJoinSesionBtnClicked()
{
	if (CGameInstance)
	{
		CGameInstance->TryFindAndJoinSession();
	}
}
