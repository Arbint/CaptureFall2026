// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STT_RotateToFaceActor.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTT_RotateToFaceActor::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find AI Controller!"))
		return EStateTreeRunStatus::Failed;
	}
	if (!InstanceData.TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Target Actor!"))
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.AIController->SetFocus(InstanceData.TargetActor);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTT_RotateToFaceActor::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find AI Controller!"))
		return EStateTreeRunStatus::Failed;
	}
	
	const FVector PawnDir = InstanceData.AIController->GetPawn()->GetActorForwardVector();
	const FVector FocalPoint = InstanceData.AIController->GetFocalPointForPriority(EAIFocusPriority::Gameplay);
	
	if (FocalPoint != FAISystem::InvalidLocation)
	{
		// CosineAngle2D returns 1 when facing the target and -1 when facing away
		const float AngleCos = PawnDir.CosineAngle2D(FocalPoint - InstanceData.AIController->GetPawn()->GetActorLocation());
		const float PrecisionCos = FMath::Cos(FMath::DegreesToRadians(InstanceData.PrecisionDegrees));
		if (AngleCos >= PrecisionCos && InstanceData.bShouldSuccess)
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}

	return EStateTreeRunStatus::Running;
}

void FSTT_RotateToFaceActor::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find AI Controller!"))
		return;
	}
	InstanceData.AIController->ClearFocus(EAIFocusPriority::Gameplay);
}
