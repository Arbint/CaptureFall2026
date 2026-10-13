// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_GetAITarget.generated.h"

/**
 * 
 */
UCLASS()
class USTT_GetAITarget : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
