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
	FMath::GetMappedRangeValueClamped()
	FillIKParams();
	const UEixCharacterMovComp* EixCharacterMovement = EixCharacterOwner->GetEixCharacterMovement();
	MoveSpeed = EixCharacterMovement->Velocity.Size();
	MovementState = EixCharacterMovement->GetMovementState();
	if (MovementState == EEixCharacterMovementState::InAir)
	{
		FallingSpeed = EixCharacterMovement->Velocity.Z;
	}
}

void UEixCharacterAnimInstance::FillIKParams()
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