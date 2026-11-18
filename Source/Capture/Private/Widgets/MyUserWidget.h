// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ConfigureWaiting(const FString& WaitingStr, bool bAllowCancel);
	FOnButtonClickedEvent& ClearAndGetButtonClickEvent();
	
private:	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* WaitingInfoText;

	UPROPERTY(meta=(BindWidget))
	class UButton* CancelBtn;
};
