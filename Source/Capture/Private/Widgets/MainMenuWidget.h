// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnFocusReceived( const FGeometry& InGeometry, const FFocusEvent& InFocusEvent ) override;

private:
	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta=(BindWidget))
	class UWaitingWidget* WaitingWidget;

	UPROPERTY()
	class UCGameInstance* CGameInstance;

	UPROPERTY(meta=(BindWidget))
	class UMatchMakingWidget* MatchMakingWidget;

	UFUNCTION()
	void SwitchToMatchMakingWidget();

	//--------------------------------//
	//            Login               //
	//--------------------------------//
private:
	UPROPERTY(meta=(BindWidget))
	class UButton* LoginBtn;

	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void LoginCompleted(bool bWasSuccessful, const FString& PlayerNickname, const FString& ErrorMsg);

	UFUNCTION()
	void WaitForNewSessionCreate();

	UFUNCTION()
	void WaitForFindAndJoinSessison();
};
