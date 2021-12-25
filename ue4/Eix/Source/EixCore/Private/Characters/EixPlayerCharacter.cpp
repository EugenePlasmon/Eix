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
}

void AEixPlayerCharacter::StopSprint()
{
}

FRotator AEixPlayerCharacter::GetAimOffset() const
{
	return ActorToWorld().InverseTransformRotation(GetBaseAimRotation().Quaternion()).Rotator();
}

void AEixPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
