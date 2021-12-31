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
	MoveSpeed = CharacterMovement->Velocity.Size();
	if (MovementState == EEixCharacterMovementState::InAir)
	{
		FallingSpeed = CharacterMovement->Velocity.Z;
	}
	CalculateLeanAmount();
	CalculateIKParams();
}

void UEixCharacterAnimInstance::CalculateLeanAmount()
{
	const UEixCharacterMovComp* CharacterMovement = EixCharacterOwner->GetEixCharacterMovement();
	if (MovementState != EEixCharacterMovementState::OnGround
		|| CharacterMovement->Velocity.IsNearlyZero())
	{
		LeanAmount = FVector2D::ZeroVector;
		return;
	}
	
	const FVector AccelerationWS = CharacterMovement->GetCurrentAcceleration();
	const bool bAccelerating = FVector::DotProduct(CharacterMovement->Velocity, AccelerationWS) > 0.f;
	const float MaxAcceleration = bAccelerating
		                              ? CharacterMovement->GetMaxAcceleration()
		                              : CharacterMovement->GetMaxBrakingDeceleration();
	const FVector RelativeAccelerationWS = AccelerationWS.GetClampedToMaxSize(MaxAcceleration) / MaxAcceleration;
	const FVector RelativeAccelerationLS = EixCharacterOwner->GetActorTransform().InverseTransformVector(RelativeAccelerationWS);

	// TODO: Depending on Gait (Walk, Jog, Sprint) adjust the amount of lean
	// Walk:	(X, Y) = (0, 0)
	// Jog:		(X, Y) = (0, MidY)
	// Sprint:	(X, Y) = (MidX, MaxY)
	LeanAmount = FVector2D(0.f, RelativeAccelerationLS.Y);
}

void UEixCharacterAnimInstance::CalculateIKParams()
{
	if (LeftFootBoneName.IsNone() || RightFootBoneName.IsNone())
	{
		return;
	}

	const UEixCharacterIKComp* IKComponent = EixCharacterOwner->GetIKComp();
	PelvisOffset = IKComponent->GetIKOffset_Pelvis();
	LeftFootEffectorLocation = ConvertIKOffsetFromWorldToBoneParent(LeftFootBoneName, IKComponent->GetIKOffset_LeftFoot());
	RightFootEffectorLocation = ConvertIKOffsetFromWorldToBoneParent(RightFootBoneName, IKComponent->GetIKOffset_RightFoot());
	LeftFootRotation = IKComponent->GetIKRotation_LeftFoot();
	RightFootRotation = IKComponent->GetIKRotation_RightFoot();
}

FVector UEixCharacterAnimInstance::ConvertIKOffsetFromWorldToBoneParent(FName BoneName, FVector IKOffsetWorld) const
{
	const USkeletalMeshComponent* SkeletalMeshComp = GetOwningComponent();
	const FVector X = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::X);
	const FVector Y = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::Y);
	const FVector Z = SkeletalMeshComp->GetBoneAxis(BoneName, EAxis::Z);
	return FVector(FVector::DotProduct(X, IKOffsetWorld), FVector::DotProduct(Y, IKOffsetWorld), FVector::DotProduct(Z, IKOffsetWorld));
}