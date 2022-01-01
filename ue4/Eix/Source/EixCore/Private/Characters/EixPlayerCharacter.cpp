#include "Characters/EixPlayerCharacter.h"
#include "Components/Movement/EixPlayerCharacterMovComp.h"

AEixPlayerCharacter::AEixPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UEixPlayerCharacterMovComp>(ACharacter::CharacterMovementComponentName)
	)
{
	EixPlayerCharacterMovement = StaticCast<UEixPlayerCharacterMovComp*>(GetCharacterMovement());
}


void AEixPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEixPlayerCharacter::MoveForward(float Value)
{
	if (FMath::IsNearlyZero(Value)
		|| !EixPlayerCharacterMovement->IsMovingOnGround()
		|| EixPlayerCharacterMovement->IsFalling())
	{
		return;
	}
	const FVector Direction = FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector();
	AddMovementInput(Direction, Value);
}

void AEixPlayerCharacter::MoveRight(float Value)
{
	if (FMath::IsNearlyZero(Value)
		|| !EixPlayerCharacterMovement->IsMovingOnGround()
		|| EixPlayerCharacterMovement->IsFalling())
	{
		return;
	}
	const FVector Direction = FRotator(0.f, GetControlRotation().Yaw, 0.f).RotateVector(FVector::RightVector);
	AddMovementInput(Direction, Value);
}

void AEixPlayerCharacter::Turn(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}
	AddControllerYawInput(Value);
}

void AEixPlayerCharacter::LookUp(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}
	AddControllerPitchInput(Value);
}

void AEixPlayerCharacter::StartSprint()
{
	SetDesiredGait(EEixGait::Sprint);
}

void AEixPlayerCharacter::StopSprint()
{
	if (GetDesiredGait() == EEixGait::Sprint)
	{
		SetDesiredGait(EEixGait::Jog);
	}
}

void AEixPlayerCharacter::StartWalk()
{
	SetDesiredGait(EEixGait::Walk);
}

void AEixPlayerCharacter::StopWalk()
{
	if (GetDesiredGait() == EEixGait::Walk)
	{
		SetDesiredGait(EEixGait::Jog);
	}
}

FRotator AEixPlayerCharacter::GetAimOffset() const
{
	return ActorToWorld().InverseTransformRotation(GetBaseAimRotation().Quaternion()).Rotator();
}

EEixGait AEixPlayerCharacter::GetCurrentAllowedGait() const
{
	if (GetDesiredGait() == EEixGait::Sprint)
	{
		return CanSprint() ? EEixGait::Sprint : EEixGait::Jog;
	}
	return GetDesiredGait();
}

bool AEixPlayerCharacter::CanSprint() const
{
	return !EixPlayerCharacterMovement->GetCurrentAcceleration().IsNearlyZero();
}

void AEixPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
