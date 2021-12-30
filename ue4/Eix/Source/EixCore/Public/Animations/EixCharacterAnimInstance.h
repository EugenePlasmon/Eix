#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/Character/EixCharacterMovementState.h"
#include "EixCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement")
	EEixCharacterMovementState MovementState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Movement|In air")
	float FallingSpeed;

#pragma region IK
/* Set in blueprint */
protected:
	UPROPERTY(EditDefaultsOnly, Category="Anim specs|IK")
	FName LeftFootBoneName;

	UPROPERTY(EditDefaultsOnly, Category="Anim specs|IK")
	FName RightFootBoneName;

/* IK params */
protected:
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

private:
	void FillIKParams();

	FVector ConvertIKOffsetFromWorldToBoneParent(FName BoneName, FVector IKOffsetWorld) const;
#pragma endregion 

#pragma region Misc
private:
	TWeakObjectPtr<class AEixCharacter> EixCharacterOwner;
#pragma endregion
};
