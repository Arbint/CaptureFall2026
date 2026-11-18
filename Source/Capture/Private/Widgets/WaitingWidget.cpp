// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WaitingWidget.h"
#include "Components/TextBlock.h"

void UWaitingWidget::ConfigureWaiting(const FString& WaitingStr, bool bAllowCancel)
{
	WaitingInfoText->SetText(FText::FromString(WaitingStr));
	CancelBtn->SetVisibility(bAllowCancel ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

FOnButtonClickedEvent& UWaitingWidget::ClearAndGetButtonClickEvent()
{
	// TODO: insert return statement here
	CancelBtn->OnClicked.Clear();
	return CancelBtn->OnClicked;
}
