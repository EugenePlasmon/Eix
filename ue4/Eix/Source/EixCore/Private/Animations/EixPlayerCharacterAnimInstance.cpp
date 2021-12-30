#include "Animations/EixPlayerCharacterAnimInstance.h"
#include "Characters/EixPlayerCharacter.h"

void UEixPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AEixPlayerCharacter>(), TEXT("UEixPlayerCharacterAnimInstance owner must be AEixPlayerCharacter"));
	EixCharacterOwner = StaticCast<AEixPlayerCharacter*>(TryGetPawnOwner());
}

void UEixPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!EixCharacterOwner.IsValid())
	{
		return;
	}
	CalculateAimAngles(DeltaSeconds);
}

void UEixPlayerCharacterAnimInstance::CalculateAimAngles(float DeltaSeconds)
{
	AimOffset = EixCharacterOwner->GetAimOffset();
	
	AimAnglePitch = FMath::FInterpTo(AimAnglePitch, AimOffset.Pitch, DeltaSeconds, AimOffsetInterpSpeed);
	
	// Measure time the player view is directed back
	const bool bPlayerWasLookingBack = bPlayerLooksBack;
	bPlayerLooksBack = FMath::Abs(AimOffset.Yaw) >= AimOffsetLookingBackMinAbsAngle;
	if (!bPlayerWasLookingBack && bPlayerLooksBack)
	{
		PlayerLookingBackTime = 0.f;
	}
	if (bPlayerWasLookingBack)
	{
		PlayerLookingBackTime += DeltaSeconds;
	}

	float TargetAimAngleYaw;
	float YawInterpSpeed;
	if (bPlayerLooksBack && PlayerLookingBackTime >= AimOffsetLookingBackMaxTime)
	{
		// Reset character head if the player view is directed back enough long
		TargetAimAngleYaw = 0.f;
		YawInterpSpeed = AimOffsetResetDirectionInterpSpeed; 
	}
	else
	{
		TargetAimAngleYaw = AimOffset.Yaw;
		YawInterpSpeed = AimOffsetInterpSpeed;
		
		// Keep looking from the same direction (do not rotate head to prevent it from unnecessary going back and forth)
		const bool bIsLookingRightBack = AimAngleYaw >= AimOffsetLookingBackMinAbsAngle;
		const bool bIsLookingLeftBack = AimAngleYaw <= -AimOffsetLookingBackMinAbsAngle;
		const bool bWantsLookRightBack = AimOffset.Yaw >= AimOffsetLookingBackMinAbsAngle;
		const bool bWantsLookLeftBack = AimOffset.Yaw <= -AimOffsetLookingBackMinAbsAngle;
		if (bKeepHeadDirectionWhileLookingBack)
		{
			if (bIsLookingRightBack && bWantsLookLeftBack)
			{
				TargetAimAngleYaw = 180.f;
			}
			else if (bIsLookingLeftBack && bWantsLookRightBack)
			{
				TargetAimAngleYaw = -180.f;
			}
		}
	}
	
	AimAngleYaw = FMath::FInterpTo(AimAngleYaw, TargetAimAngleYaw, DeltaSeconds, YawInterpSpeed);
	
	UE_LOG(LogTemp, Warning, TEXT("Current %f, Target %f"), AimAngleYaw, TargetAimAngleYaw);
}
