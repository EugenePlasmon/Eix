#pragma once

#include "CoreMinimal.h"
#include "EixCustomMovementMode.generated.h"

UENUM(BlueprintType)
enum EEixCustomMovementMode
{
	CMOVE_None		UMETA(DisplayName = "None"),
	CMOVE_Rolling	UMETA(DisplayName = "Rolling"),
	CMOVE_Max		UMETA(Hidden)
};