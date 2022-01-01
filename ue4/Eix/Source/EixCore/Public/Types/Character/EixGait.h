#pragma once

#include "CoreMinimal.h"
#include "EixGait.Generated.h"

UENUM(BlueprintType)
enum class EEixGait: uint8
{
	Walk,
	Jog,
	Sprint
};