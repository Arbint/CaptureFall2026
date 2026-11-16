// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenuWidget.h"
#include "Widgets/MatchMakingWidget.h"
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
