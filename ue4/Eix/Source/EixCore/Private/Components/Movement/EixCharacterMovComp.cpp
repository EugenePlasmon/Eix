#include "Components/Movement/EixCharacterMovComp.h"
#include "Characters/EixCharacter.h"

#include "GameFramework/Character.h"
#include "Types/Character/EixCustomMovementMode.h"

UEixCharacterMovComp::UEixCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
	CacheCharacterOwner();
	ReadMovementSpecs();
	MaxWalkSpeed = MovementSpecs.GetMaxSpeedForGait(CurrentGait);
	PrevVelocity = Velocity;
	PrevActorRotation = EixCharacterOwner->GetActorRotation();
}

void UEixCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCharacterMovementValues(DeltaTime);
}

void UEixCharacterMovComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (IsMovingOnGround())
	{
		SetMovementState(EEixMovementState::OnGround);
	}
	else if (IsFalling())
	{
		SetMovementState(EEixMovementState::InAir);
	}
	else if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case CMOVE_Rolling:
			SetMovementState(EEixMovementState::Rolling);
			break;
		default:
			break;
		}
	}
}

EEixGait UEixCharacterMovComp::GetCurrentActualGait() const
{
	constexpr float Tolerance = 1.f; // don't need to be too precise in this calculation
	if (FMath::IsNearlyEqual(MoveSpeed, MovementSpecs.GetMaxSpeedForGait(CurrentGait), Tolerance))
	{
		return CurrentGait;
	}
	if (MoveSpeed < MovementSpecs.MaxWalkSpeed + Tolerance)
	{
		return EEixGait::Walk;
	}
	if (MoveSpeed < MovementSpecs.MaxJogSpeed + Tolerance)
	{
		return EEixGait::Jog;
	}
	return EEixGait::Sprint;
}

void UEixCharacterMovComp::SetCurrentGait(EEixGait In_CurrentGait)
{
	if (CurrentGait == In_CurrentGait)
	{
		return;
	}
	CurrentGait = In_CurrentGait;
	MaxWalkSpeed = MovementSpecs.GetMaxSpeedForGait(CurrentGait);
}

void UEixCharacterMovComp::SetMovementState(EEixMovementState NewMovementState)
{
	if (NewMovementState == EixMovementState)
	{
		return;
	}

	const EEixMovementState PrevMovementState = EixMovementState;
	EixMovementState = NewMovementState;
	if (OnMovementStateChanged.IsBound())
	{
		OnMovementStateChanged.Broadcast(PrevMovementState, NewMovementState);
	}
}

void UEixCharacterMovComp::CacheCharacterOwner()
{
	checkf(GetCharacterOwner()->IsA<AEixCharacter>(), TEXT("UEixCharacterMovComp owner must be AEixCharacter"));
	EixCharacterOwner = StaticCast<AEixCharacter*>(GetCharacterOwner());
}

void UEixCharacterMovComp::ReadMovementSpecs()
{
	FEixMovementSpecs* FoundSpecs = MovementSpecsTable.DataTable->FindRow<FEixMovementSpecs>(
		MovementSpecsTable.RowName, GetFullName());
	check(FoundSpecs);
	MovementSpecs = *FoundSpecs;
}

void UEixCharacterMovComp::UpdateCharacterMovementValues(float DeltaTime)
{
	MoveSpeed = Velocity.Size2D();
	VelocityAcceleration = (Velocity - PrevVelocity) / DeltaTime;
	VelocityAcceleration_LS = PrevActorRotation.UnrotateVector(VelocityAcceleration);
	
	PrevVelocity = Velocity;
	PrevActorRotation = EixCharacterOwner->GetActorRotation();
}
