// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Tasks/StateTreeAITask.h"
#include "STT_RotateToFaceActor.generated.h"

class AAIController;

USTRUCT()
struct FStateTreeRotateToFaceActoInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	AAIController* AIController = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter)
	float Precision = 1.f;

	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bShouldSuccess = true;
};

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Rotate To Face Target", Category = "AI|Action"))
struct FSTT_RotateToFaceActor : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()
	using FInstanceDataType = FStateTreeRotateToFaceActoInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
