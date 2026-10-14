// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once

#include "CoreMinimal.h"
#include "Tasks/StateTreeAITask.h"
#include "AbilitySystem/CAbilityInputID.h"
#include "STT_PressAbilityInput.generated.h"

class AAIController;
class UAbilitySystemComponent;

USTRUCT()
struct FStateTreePressAbilityInputInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = Parameter)
	ECAbilityInputID InputToPress;
};

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Rotate To Face Target", Category = "AI|Action"))
struct FSTT_PressAbilityInput : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()
	using FInstanceDataType = FStateTreePressAbilityInputInstanceData;
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
