#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EixCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma region IK
/* Set in blueprint */
protected:
	UPROPERTY(EditDefaultsOnly)
	FName LeftFootBoneName;

	UPROPERTY(EditDefaultsOnly)
	FName RightFootBoneName;

/* IK params */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector PelvisOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector LeftFootEffectorLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RightFootEffectorLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator LeftFootRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
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
