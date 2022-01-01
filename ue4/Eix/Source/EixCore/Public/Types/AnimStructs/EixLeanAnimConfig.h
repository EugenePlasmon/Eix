#pragma once

#include "CoreMinimal.h"
#include "Types/Character/EixGait.h"
#include "EixLeanAnimConfig.Generated.h"

USTRUCT(BlueprintType)
struct FEixLeanAnimConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LeanAmountInterpSpeed = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D LeanAmountModifier_Walk = FVector2D::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D LeanAmountModifier_Jog = FVector2D::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D LeanAmountModifier_Sprint = FVector2D::ZeroVector;

	FVector2D LeanAmountModifierForGait(EEixGait Gait) const;
};

inline FVector2D FEixLeanAnimConfig::LeanAmountModifierForGait(EEixGait Gait) const
{
	switch (Gait)
	{
	case EEixGait::Walk:
		return LeanAmountModifier_Walk;
	case EEixGait::Jog:
		return LeanAmountModifier_Jog;
	case EEixGait::Sprint:
		return LeanAmountModifier_Sprint;
	default:
		return FVector2D::ZeroVector;
	}
}
