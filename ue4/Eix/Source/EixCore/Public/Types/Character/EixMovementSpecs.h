#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Types/Character/EixGait.h"
#include "EixMovementSpecs.Generated.h"

USTRUCT(BlueprintType)
struct FEixMovementSpecs: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxWalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxJogSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSprintSpeed = 0.f;

	float GetMaxSpeedForGait(EEixGait Gait) const;
};

inline float FEixMovementSpecs::GetMaxSpeedForGait(EEixGait Gait) const
{
	switch (Gait)
	{
	case EEixGait::Walk:
		return MaxWalkSpeed;
	case EEixGait::Jog:
		return MaxJogSpeed;
	case EEixGait::Sprint:
		return MaxSprintSpeed;
	default:
		return 0.f;
	}
}
