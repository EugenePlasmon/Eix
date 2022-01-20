#pragma once

#include "CoreMinimal.h"
#include "EixMeleeWeaponCategory.Generated.h"

UENUM(BlueprintType)
enum class EEixMeleeWeaponCategory: uint8
{
	Sword,
	TwoBlades,
	Cleaver,
	Axe
};