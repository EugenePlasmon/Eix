#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/Character/EixCharacterMovementState.h"
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
	UPROPERTY(EditDefaultsOnly, Category="Configuration|IK")
	FName LeftFootBoneName;

	UPROPERTY(EditDefaultsOnly, Category="Configuration|IK")
	FName RightFootBoneName;
#pragma endregion

#pragma region CalculatedParams
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	EEixCharacterMovementState MovementState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement|In air")
	float FallingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement|Lean")
	FVector2D LeanAmount;
	
/* IK params */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FVector PelvisOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FVector LeftFootEffectorLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FVector RightFootEffectorLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FRotator LeftFootRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|IK")
	FRotator RightFootRotation;
/* ~IK params */
#pragma endregion

private:
	void CalculateLeanAmount();
	
	void CalculateIKParams();

	FVector ConvertIKOffsetFromWorldToBoneParent(FName BoneName, FVector IKOffsetWorld) const;

	TWeakObjectPtr<class AEixCharacter> EixCharacterOwner;
};
