#include "Components/Movement/EixPlayerCharacterMovComp.h"
#include "Characters/EixPlayerCharacter.h"
#include "Types/Character/EixCustomMovementMode.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UEixPlayerCharacterMovComp::UEixPlayerCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixPlayerCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(GetCharacterOwner()->IsA<AEixCharacter>(), TEXT("UEixCharacterMovComp owner must be AEixCharacter"));
	EixPlayerCharacterOwner = StaticCast<AEixPlayerCharacter*>(GetCharacterOwner());
}

void UEixPlayerCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEixPlayerCharacterMovComp::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case CMOVE_Rolling:
			PhysRolling(DeltaTime, Iterations);
			break;
		default:
			break;
		}
	}
	
	Super::PhysCustom(DeltaTime, Iterations);
}

void UEixPlayerCharacterMovComp::PhysicsRotation(float DeltaTime)
{
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case CMOVE_Rolling:
			PhysicsRotationRolling(DeltaTime);
			return;
		default:
			return;
		}
	}
	
	Super::PhysicsRotation(DeltaTime);
}

bool UEixPlayerCharacterMovComp::IsRolling() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CMOVE_Rolling;
}

bool UEixPlayerCharacterMovComp::CanStartRolling() const
{
	return IsMovingOnGround();
}

void UEixPlayerCharacterMovComp::StartRolling()
{
	// must be set in BP
	check(!FMath::IsNearlyZero(RollingMaxDuration) && IsValid(RollingSpeedCurve) && IsValid(RollingRotationRateYawCurve));
	
	// set initial values
	InitialRollingSpeedXY = Velocity.Size2D();
	CurrentRollingSpeedZ = 0.f;
	CurrentRollingAccumulatedTime = 0.f;
	Velocity = FVector::ZeroVector;

	GetWorld()->GetTimerManager().SetTimer(RollingTimer, FTimerDelegate::CreateLambda([this]
	{
		if (IsValid(this))
		{
			this->StopRolling();
		}
	}), RollingMaxDuration, false);

	SetMovementMode(MOVE_Custom, CMOVE_Rolling);
}

void UEixPlayerCharacterMovComp::StopRolling()
{
	if (!IsRolling())
	{
		return;
	}
	// We're sure we stop rolling on ground, so here we always enable MOVE_Walking.
	// If rolling ends in air, we would have already found it in PhysRolling(...) at sweep tests
	SetMovementMode(MOVE_Walking);
}

void UEixPlayerCharacterMovComp::PhysRolling(float DeltaTime, int32 Iterations)
{
	// 1) Vertical sweep test 1: check for ground under the capsule, add vertical component of velocity if falling.
	constexpr float SweepDistance = 2.f;
	FHitResult FallingCheckHitResult;
	const bool bIsFalling = !SweepCharacterCapsuleSingleByChannel(
		FallingCheckHitResult, -SweepDistance * FVector::UpVector, ECC_Visibility
	);
	if (bIsFalling)
	{
		CurrentRollingSpeedZ += GetGravityZ() * DeltaTime;
	}
	else
	{
		CurrentRollingSpeedZ = 0.f;
	}

	// 2) Move character.
	const float RelativeTime = CurrentRollingAccumulatedTime / RollingMaxDuration;
	const float TargetRollingSpeedXY = RollingSpeedCurve->GetFloatValue(RelativeTime);
	const float CurrentRollingSpeedXY = InterpRollingSpeedXY(TargetRollingSpeedXY);
	const FVector Direction = EixPlayerCharacterOwner->GetActorForwardVector();
	const FRotator Rotation = EixPlayerCharacterOwner->GetActorRotation();
	const FVector CurrentRollingVelocity = CurrentRollingSpeedXY * Direction + CurrentRollingSpeedZ * FVector::UpVector;
	const FVector DeltaLoc = CurrentRollingVelocity * DeltaTime;
	const FVector CurrentLocation = EixPlayerCharacterOwner->GetActorLocation();
	
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(DeltaLoc, Rotation, true, Hit);
	if (Hit.IsValidBlockingHit())
	{
		HandleImpact(Hit, DeltaTime, DeltaLoc);
		SlideAlongSurface(DeltaLoc, 1.f - Hit.Time, Hit.Normal, Hit, true);
		
		// Refine vertical speed by calculating actual distance passed (could be different if slide along surface happened)
		const FVector NewLocation = EixPlayerCharacterOwner->GetActorLocation();
		const float ActualVelocityZ = (NewLocation.Z - CurrentLocation.Z) / DeltaTime;
		CurrentRollingSpeedZ = FMath::Min(0.f, ActualVelocityZ);
	}

	// 3) Vertical sweep test 2: if character is falling
	// find whether the rolling animation will end before the character lands.
	// If so, setting MovementMode to Falling and transfer current rolling velocity to falling initial velocity.
	if (bIsFalling && RelativeTime > 0.5f) // we don't want to start these checks too soon
	{
		const float RollingRemainingTime = FMath::Max(0.f, RollingMaxDuration - CurrentRollingAccumulatedTime);
		const float FreeFallHeightUntilRollingEnds = -CurrentRollingSpeedZ * RollingRemainingTime
			- 0.5f * GetGravityZ() * FMath::Square(RollingRemainingTime); // 0 = h + v0*t + g*t^2/2
		constexpr float MinHeightToChangeMovementMode = 10.f;
		if (FreeFallHeightUntilRollingEnds >= MinHeightToChangeMovementMode)
		{
			FHitResult InAirCheckHitResult;
			const bool bInAir = !SweepCharacterCapsuleSingleByChannel(
				InAirCheckHitResult, -FreeFallHeightUntilRollingEnds * FVector::UpVector, ECC_Visibility
			);
			if (bInAir)
			{
				SetMovementMode(MOVE_Falling);
				EixPlayerCharacterOwner->LaunchCharacter(CurrentRollingVelocity, true, true);
			}
		}
	}

	CurrentRollingAccumulatedTime += DeltaTime;
}

void UEixPlayerCharacterMovComp::PhysicsRotationRolling(float DeltaTime)
{
	if (Acceleration.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		// Don't change rotation if there is no acceleration.
		return;
	}
	
	const float RelativeTime = CurrentRollingAccumulatedTime / RollingMaxDuration;
	const FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	float DesiredYaw = Acceleration.GetSafeNormal2D().Rotation().Yaw;
	DesiredYaw = FRotator::NormalizeAxis(DesiredYaw);
	const float RollingRotationRate = RollingRotationRateYawCurve->GetFloatValue(RelativeTime);
	const float DeltaYaw = RollingRotationRate * DeltaTime;

	constexpr float AngleTolerance = 1e-3f;
	if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredYaw, AngleTolerance))
	{
		DesiredYaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredYaw, DeltaYaw);
		// Set the new rotation.
		const FRotator NewRotation(CurrentRotation.Pitch, DesiredYaw, CurrentRotation.Roll);
		MoveUpdatedComponent(FVector::ZeroVector, NewRotation, false);
	}
}

bool UEixPlayerCharacterMovComp::SweepCharacterCapsuleSingleByChannel(FHitResult& OutHit, const FVector& EndStartDelta,
                                                                      ECollisionChannel Channel) const
{
	const UCapsuleComponent* CapsuleComponent = EixPlayerCharacterOwner->GetCapsuleComponent();
	float CapsuleRadius, CapsuleHalfHeight;
	CapsuleComponent->GetUnscaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	const FVector Start = CapsuleComponent->GetComponentLocation();
	const FVector End = Start + EndStartDelta;
	const FQuat Quat = CapsuleComponent->GetComponentQuat();
	const FCollisionShape Shape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	return GetWorld()->SweepSingleByChannel(OutHit, Start, End, Quat, Channel, Shape);
}

float UEixPlayerCharacterMovComp::InterpRollingSpeedXY(float TargetRollingSpeedXY) const
{
	// Start with initial speed, interp to target speed (from curve) in time RollingSpeedXYInitialInterpTime
	const float RollingSpeedXYInitialInterpTime = RollingMaxDuration / 10.f;
	const float RollingSpeedXYInitialInterpAlpha = FMath::GetMappedRangeValueClamped(
		FVector2D(0.f, RollingSpeedXYInitialInterpTime),
		FVector2D(0.f, 1.f), CurrentRollingAccumulatedTime
	);
	return FMath::Lerp(InitialRollingSpeedXY, TargetRollingSpeedXY, RollingSpeedXYInitialInterpAlpha);
}