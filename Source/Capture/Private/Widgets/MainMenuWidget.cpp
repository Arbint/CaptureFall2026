// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenuWidget.h"
#include "Widgets/MatchMakingWidget.h"
#include "Widgets/WaitingWidget.h"
#include "Framework/CGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CGameInstance = GetGameInstance<UCGameInstance>();
	if (CGameInstance)
	{
		CGameInstance->OnAuthLoginCompletedDelegate.AddUObject(this, &UMainMenuWidget::LoginCompleted);
		if (CGameInstance->IsLoggedIn())
		{
			SwitchToMatchMakingWidget();
		}
	}

	LoginBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::LoginBtnClicked);

	MatchMakingWidget->GetFindAndJoinSessionBtnClickEvent().AddDynamic(this, &UMainMenuWidget::WaitForFindAndJoinSessison);
	MatchMakingWidget->GetCreateNewSessionBtnClickEvent().AddDynamic(this, &UMainMenuWidget::WaitForNewSessionCreate);
}

FReply UMainMenuWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Focus Added to Path"))
	UWidget* ActiveWidget = MainSwitcher->GetActiveWidget();
	if (ActiveWidget)
	{
		ActiveWidget->SetKeyboardFocus();
		return FReply::Handled();
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UMainMenuWidget::SwitchToMatchMakingWidget()
{
	MainSwitcher->SetActiveWidget(MatchMakingWidget);
	MatchMakingWidget->SetKeyboardFocus();
}

void UMainMenuWidget::LoginBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Logging in"))
	if (CGameInstance)
	{
		if (CGameInstance->IsLoggedIn() || CGameInstance->IsLoggingIn())
		{
			UE_LOG(LogTemp, Warning, TEXT("Log in not allowed at this time, already logged in or loggin in"))
			return;
		}

		CGameInstance->ClientAuthPortalLogin();
		MainSwitcher->SetActiveWidget(WaitingWidget);
		WaitingWidget->ConfigureWaiting("Logging In", false);
	}
}

void UMainMenuWidget::LoginCompleted(bool bWasSuccessful, const FString& PlayerNickname, const FString& ErrorMsg)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Log in Successful!"))
		SwitchToMatchMakingWidget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"))
	}
}

void UMainMenuWidget::WaitForNewSessionCreate()
{
	MainSwitcher->SetActiveWidget(WaitingWidget);
	WaitingWidget->ConfigureWaiting("Creating Game", true);
	WaitingWidget->ClearAndGetButtonClickEvent().AddDynamic(this, &UMainMenuWidget::SwitchToMatchMakingWidget);
	WaitingWidget->SetKeyboardFocus();
}

void UMainMenuWidget::WaitForFindAndJoinSessison()
{
	MainSwitcher->SetActiveWidget(WaitingWidget);
	WaitingWidget->ConfigureWaiting("Finding Game", true);
	WaitingWidget->ClearAndGetButtonClickEvent().AddDynamic(this, &UMainMenuWidget::SwitchToMatchMakingWidget);
	WaitingWidget->SetKeyboardFocus();
}
