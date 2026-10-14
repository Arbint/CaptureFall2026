// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT_GetAITarget.h"
#include "StateTreeExecutionContext.h"
#include "AI/CAIController.h"

EStateTreeRunStatus USTT_GetAITarget::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) 
{
	FStateTreeDataView DataView = Context.GetContextDataByName("AIController");
	if (const ACAIController* AIController = DataView.GetPtr<ACAIController>())
	{
	}

	return EStateTreeRunStatus::Succeeded;
}
