// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MyUserWidget.h"
#include "Components/TextBlock.h"

void UMyUserWidget::ConfigureWaiting(const FString& WaitingStr, bool bAllowCancel)
{
	WaitingInfoText->SetText(FText::FromString(WaitingStr));
	CancelBtn->SetVisibility(bAllowCancel ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

FOnButtonClickedEvent& UMyUserWidget::ClearAndGetButtonClickEvent()
{
	// TODO: insert return statement here
	CancelBtn->OnClicked.Clear();
	return CancelBtn->OnClicked;
}
