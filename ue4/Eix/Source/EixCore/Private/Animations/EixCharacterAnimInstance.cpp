#include "Animations/EixCharacterAnimInstance.h"
#include "Characters/EixCharacter.h"
#include "Components/EixCharacterIKComp.h"
#include "Components/Movement/EixCharacterMovComp.h"

void UEixCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AEixCharacter>(), TEXT("UEixCharacterAnimInstance owner must be AEixCharacter"));
	EixCharacterOwner = StaticCast<AEixCharacter*>(TryGetPawnOwner());
}

void UEixCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!EixCharacterOwner.IsValid())
	{
		return;
	}
	const UEixCharacterMovComp* CharacterMovement = EixCharacterOwner->GetEixCharacterMovement();
	MovementState = CharacterMovement->GetMovementState();
	CurrentGait = CharacterMovement->GetCurrentGait();
	CurrentActualGait = CharacterMovement->GetCurrentActualGait();
	MoveSpeed = CharacterMovement->GetMoveSpeed();
	if (MovementState == EEixMovementState::InAir)
	{
		// Set falling speed only when in air to save the last in-air speed value and use it in grounded state
		FallingSpeed = CharacterMovement->Velocity.Z;
	}
	RelativeAcceleration = CalculateRelativeAcceleration();
	LeanAmount = CalculateLeanAmount(DeltaSeconds);
	FootIKAnimParams = CalculateFootIKParams();
}

FVector UEixCharacterAnimInstance::CalculateRelativeAcceleration() const
{
	const UEixCharacterMovComp* CharacterMovement = EixCharacterOwner->GetEixCharacterMovement();
	const FVector Acceleration = CharacterMovement->GetVelocityAcceleration_LS();
	if (Acceleration.IsNearlyZero(0.01f))
	{
		return FVector::ZeroVector;
	}
	// if false - then decelerating
	const bool bAccelerating = FVector::DotProduct(CharacterMovement->Velocity, Acceleration) > 0.f;
	const float MaxAcceleration = bAccelerating
		                              ? CharacterMovement->GetMaxAcceleration()
		                              : CharacterMovement->GetMaxBrakingDeceleration();
	return Acceleration.GetClampedToMaxSize(MaxAcceleration) / MaxAcceleration;
}

FVector2D UEixCharacterAnimInstance::CalculateLeanAmount(float DeltaSeconds) const
{
	const UEixCharacterMovComp* CharacterMovement = EixCharacterOwner->GetEixCharacterMovement();
	if (MovementState != EEixMovementState::OnGround
		|| CharacterMovement->Velocity.IsNearlyZero())
	{
		return FVector2D::ZeroVector;
	}
	const FVector2D TargetLeanAmount =
		LeanConfiguration.LeanAmountModifierForGait(CurrentGait) * FVector2D(RelativeAcceleration);
	return FMath::Vector2DInterpTo(LeanAmount,
	                               TargetLeanAmount,
	                               DeltaSeconds,
	                               LeanConfiguration.LeanAmountInterpSpeed);
}

FEixFootIKAnimParams UEixCharacterAnimInstance::CalculateFootIKParams() const
{
	check(!LeftFootBoneName.IsNone() && !RightFootBoneName.IsNone());
	const UEixCharacterIKComp* IKComponent = EixCharacterOwner->GetIKComp();
	return FEixFootIKAnimParams(
		IKComponent->GetIKOffset_Pelvis(),
		ConvertIKOffsetFromWorldToBoneParent(LeftFootBoneName, IKComponent->GetIKOffset_LeftFoot()),
		ConvertIKOffsetFromWorldToBoneParent(RightFootBoneName, IKComponent->GetIKOffset_RightFoot()),
		IKComponent->GetIKRotation_LeftFoot(),
		IKComponent->GetIKRotation_RightFoot()
	);
}

FVector UEixCharacterAnimInstance::ConvertIKOffsetFromWorldToBoneParent(FName BoneName, FVector IKOffsetWorld) const
{
	const USkeletalMeshComponent* SkeletalMeshComp = GetOwningComponent();
	const FVector X = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::X);
	const FVector Y = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::Y);
	const FVector Z = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::Z);
	return FVector(FVector::DotProduct(X, IKOffsetWorld), FVector::DotProduct(Y, IKOffsetWorld), FVector::DotProduct(Z, IKOffsetWorld));
}