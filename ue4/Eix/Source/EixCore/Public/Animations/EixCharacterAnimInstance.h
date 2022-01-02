#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/AnimStructs/EixFootIKAnimParams.h"
#include "Types/AnimStructs/EixLeanAnimConfig.h"
#include "Types/Character/EixGait.h"
#include "Types/Character/EixMovementState.h"
#include "EixCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

#pragma region Overriden
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion

#pragma region Configuration
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Configuration|IK")
	FName LeftFootBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Configuration|IK")
	FName RightFootBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Configuration|Lean")
	FEixLeanAnimConfig LeanConfiguration;
#pragma endregion

#pragma region CalculatedParams
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	EEixMovementState MovementState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	EEixGait CurrentGait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	EEixGait CurrentActualGait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement|In air")
	float FallingSpeed;
	
	/**
	 * In local space.
	 * Coordinate values are normalized to range (-1, 1) where 1 means max acceleration and -1 means max deceleration
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	FVector RelativeAcceleration;

	/**
	 * X value is a lean amount in forward/backward direction and Y value is in left/right direction in local space.
	 * Coordinate values are normalized to range (-1, 1) where 1 and -1 mean max lean amount in a corresponding direction
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement|Lean")
	FVector2D LeanAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FEixFootIKAnimParams FootIKAnimParams;
#pragma endregion

private:
	FVector CalculateRelativeAcceleration() const;
	
	FVector2D CalculateLeanAmount(float DeltaSeconds) const;
	
	FEixFootIKAnimParams CalculateFootIKParams() const;

	FVector ConvertIKOffsetFromWorldToBoneParent(FName BoneName, FVector IKOffsetWorld) const;

	TWeakObjectPtr<class AEixCharacter> EixCharacterOwner;
};
