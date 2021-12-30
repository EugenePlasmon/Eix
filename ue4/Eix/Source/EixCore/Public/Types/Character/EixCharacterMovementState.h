#pragma once

#include "CoreMinimal.h"
#include "EixCharacterMovementState.Generated.h"

UENUM(BlueprintType)
enum class EEixCharacterMovementState: uint8
{
	None,
	OnGround,
	InAir,
	Ragdoll
};