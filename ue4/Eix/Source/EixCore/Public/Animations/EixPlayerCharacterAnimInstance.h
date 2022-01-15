#pragma once

#include "CoreMinimal.h"
#include "Animations/EixCharacterAnimInstance.h"
#include "EixPlayerCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixPlayerCharacterAnimInstance : public UEixCharacterAnimInstance
{
	GENERATED_BODY()

#pragma region Overriden
public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion

#pragma region Configuration
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration|Aim offset")
	float AimOffsetLookingBackMinAbsAngle = 130.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration|Aim offset")
	bool bKeepHeadDirectionWhileLookingBack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration|Aim offset")
	float AimOffsetLookingBackMaxTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration|Aim offset")
	float AimOffsetInterpSpeed = 4.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration|Aim offset")
	float AimOffsetResetDirectionInterpSpeed = 2.f;
#pragma endregion

#pragma region CalculatedParams
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Aim offset")
	FRotator AimOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Aim offset")
	float AimAngleYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Aim offset")
	float AimAnglePitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculated params|Attack")
	bool bIsAttacking;
#pragma endregion
	
private:
	bool bPlayerLooksBack = false;
	float PlayerLookingBackTime = 0.f;
	
	void CalculateAimAngles(float DeltaSeconds);
	
	TWeakObjectPtr<class AEixPlayerCharacter> EixCharacterOwner;
};
