#include "Components/Movement/EixCharacterMovComp.h"
#include "Characters/EixCharacter.h"

#include "GameFramework/Character.h"

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
	
	VelocityAcceleration = (Velocity - PrevVelocity) / DeltaTime;
	VelocityAcceleration_LS = PrevActorRotation.UnrotateVector(VelocityAcceleration);
	
	PrevVelocity = Velocity;
	PrevActorRotation = EixCharacterOwner->GetActorRotation();
}

void UEixCharacterMovComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (IsMovingOnGround())
	{
		MovementState = EEixMovementState::OnGround;
	}
	else if (IsFalling())
	{
		MovementState = EEixMovementState::InAir;
	}
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

