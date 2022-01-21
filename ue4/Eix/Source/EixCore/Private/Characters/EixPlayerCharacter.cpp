#include "Characters/EixPlayerCharacter.h"

#include "Components/Movement/EixPlayerCharacterMovComp.h"
#include "Components/EixCharacterMeleeCombatComp.h"

AEixPlayerCharacter::AEixPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UEixPlayerCharacterMovComp>(ACharacter::CharacterMovementComponentName)
	)
{
	EixPlayerCharacterMovement = StaticCast<UEixPlayerCharacterMovComp*>(GetCharacterMovement());
	MeleeCombatComp = CreateDefaultSubobject<UEixCharacterMeleeCombatComp>(TEXT("MeleeCombatComponent"));
}

void AEixPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEixPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEixPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (!CanMeleeAttack() && MeleeCombatComp->IsPlayingAttack())
	{
		MeleeCombatComp->HaltCombo(true);
	}
}

void AEixPlayerCharacter::MoveForward(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	if (MeleeCombatComp->IsAttackFading())
	{
		MeleeCombatComp->HaltCombo();
	}
	
	const FVector Direction = FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector();
	AddMovementInput(Direction, Value);
}

void AEixPlayerCharacter::MoveRight(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	if (MeleeCombatComp->IsAttackFading())
	{
		MeleeCombatComp->HaltCombo();
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

void AEixPlayerCharacter::Roll()
{
	if (!EixPlayerCharacterMovement->CanStartRolling())
	{
		return;
	}

	if (MeleeCombatComp->IsAttackExecuting())
	{
		return;
	}
	if (MeleeCombatComp->IsAttackFading())
	{
		MeleeCombatComp->HaltCombo();
	}
	
	EixPlayerCharacterMovement->StartRolling();
	OnStartRolling();
}

void AEixPlayerCharacter::PrimaryAttack(EInputEvent InputEvent)
{
	if (CanMeleeAttack())
	{
		MeleeCombatComp->PrimaryAttack(InputEvent);
	}
}

void AEixPlayerCharacter::SecondaryAttack(EInputEvent InputEvent)
{
	if (CanMeleeAttack())
	{
		MeleeCombatComp->SecondaryAttack(InputEvent);
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

bool AEixPlayerCharacter::CanMeleeAttack() const
{
	return EixPlayerCharacterMovement->IsMovingOnGround();
}
