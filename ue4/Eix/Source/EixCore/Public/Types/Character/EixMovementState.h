#pragma once

#include "CoreMinimal.h"
#include "EixMovementState.Generated.h"

UENUM(BlueprintType)
enum class EEixMovementState: uint8
{
	None,
	OnGround,
	Rolling,
	InAir,
	Ragdoll
};