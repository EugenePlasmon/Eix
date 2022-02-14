#pragma once

#include "CoreMinimal.h"
#include "Types/Character/EixGait.h"
#include "Types/Character/EixMovementState.h"
#include "EixMeleeAttackPerformingInfo.Generated.h"

// TODO: rename: PriorInfo or PriorState
USTRUCT(BlueprintType)
struct FEixMeleeAttackPerformingInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EInputEvent> InputEvent;

	UPROPERTY()
	EEixMovementState CurrentMovementState;

	UPROPERTY()
	EEixGait CurrentGait;
	
	UPROPERTY()
	FVector CurrentInputAcceleration;

	FEixMeleeAttackPerformingInfo()
		: InputEvent(IE_Pressed), CurrentMovementState(EEixMovementState::None), CurrentGait(EEixGait::Jog),
		  CurrentInputAcceleration(FVector::ZeroVector)
	{
	}

	FEixMeleeAttackPerformingInfo(EInputEvent InputEvent, EEixMovementState CurrentMovementState, EEixGait CurrentGait,
	                              FVector CurrentInputAcceleration)
		: InputEvent(InputEvent), CurrentMovementState(CurrentMovementState), CurrentGait(CurrentGait),
		  CurrentInputAcceleration(CurrentInputAcceleration)
	{
	}
};
