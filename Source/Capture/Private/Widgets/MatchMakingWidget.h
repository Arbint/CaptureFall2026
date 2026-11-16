// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchMakingWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMatchMakingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	class UButton* CreateNewSessionBtn;

	UPROPERTY(meta=(BindWidget))
	class UButton* FindAndJoinSessionBtn;

	UPROPERTY()
	class UCGameInstance* CGameInstance;

	UFUNCTION()
	void CreateNewSessionBtnClicked();

	UFUNCTION()
	void FindAndJoinSesionBtnClicked();
};
