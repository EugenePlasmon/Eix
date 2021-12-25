#pragma once

#include "CoreMinimal.h"
#include "Animations/EixCharacterAnimInstance.h"
#include "EixPlayerCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixPlayerCharacterAnimInstance : public UEixCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim specs|Aim offset")
	float AimOffsetLookingBackMinAbsAngle = 135.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim specs|Aim offset")
	bool bKeepHeadDirectionWhileLookingBack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim specs|Aim offset")
	float AimOffsetLookingBackMaxTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim specs|Aim offset")
	float AimOffsetInterpSpeed = 4.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Anim specs|Aim offset")
	float AimOffsetResetDirectionInterpSpeed = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params")
	FRotator AimOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params")
	float AimAngleYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params")
	float AimAnglePitch;
	
private:
	bool bPlayerLooksBack = false;
	float PlayerLookingBackTime = 0.f;
	
	void CalculateAimAngles(float DeltaSeconds);
	
#pragma region Misc
private:
	TWeakObjectPtr<class AEixPlayerCharacter> EixCharacterOwner;
#pragma endregion
};
