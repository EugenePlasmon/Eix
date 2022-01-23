#include "Components/EixCharacterMeleeCombatComp.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/Movement/EixPlayerCharacterMovComp.h"

UEixCharacterMeleeCombatComp::UEixCharacterMeleeCombatComp()
{
}

void UEixCharacterMeleeCombatComp::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA<AEixPlayerCharacter>());
	EixCharacterOwner = StaticCast<AEixPlayerCharacter*>(GetOwner());
	GetOwnerAnimInstance()->OnMontageEnded
	                      .AddDynamic(this, &UEixCharacterMeleeCombatComp::OnMontageEnded);
	EixCharacterOwner->GetEixPlayerCharacterMovement()->OnMovementStateChanged
	                 .AddDynamic(this, &UEixCharacterMeleeCombatComp::OnMovementStateChanged);
}

void UEixCharacterMeleeCombatComp::PrimaryAttack(const FEixMeleeAttackPerformingInfo& Info)
{
	if (Info.CurrentMovementState == EEixMovementState::OnGround
		&& Info.InputEvent == IE_Pressed)
	{
		if (State.bCanExecuteAttack)
		{
			const bool bOnRun = !Info.CurrentInputAcceleration.IsNearlyZero() && Info.CurrentGait == EEixGait::Sprint;
			UAnimMontage* AttackMontage = bOnRun ? OnRunAttackConfig.PrimaryAttackMontage : GetPrimaryAttackMontage();
			ExecuteAttack(AttackMontage);
		}
		else if (State.bInComboWindow)
		{
			State.bWantsExecuteNextComboPrimaryAttack = true;
			State.bWantsExecuteNextComboSecondaryAttack = false;
		}
	}
	else if (Info.CurrentMovementState == EEixMovementState::Rolling
		&& Info.InputEvent == IE_Pressed
		&& State.bInRollingAttackWindow)
	{
		State.bWantsExecutePrimaryAttackAfterRolling = true;
		if (State.bCanExecuteAttackWhileRolling)
		{
			ProceedRollingAttack();
		}
	}
}

void UEixCharacterMeleeCombatComp::SecondaryAttack(const FEixMeleeAttackPerformingInfo& Info)
{
	if (Info.CurrentMovementState == EEixMovementState::OnGround)
	{
		State.bSecondaryAttackReleased = Info.InputEvent == IE_Released;
		
		if (Info.InputEvent == IE_Pressed)
		{
			if (State.bCanExecuteAttack)
			{
				const bool bOnRun = !Info.CurrentInputAcceleration.IsNearlyZero() && Info.CurrentGait == EEixGait::Sprint;
				UAnimMontage* AttackMontage = bOnRun ? OnRunAttackConfig.SecondaryAttackMontage : GetSecondaryAttackMontage();
				ExecuteAttack(AttackMontage);
			}
			else if (State.bInComboWindow)
			{
				State.bWantsExecuteNextComboPrimaryAttack = false;
				State.bWantsExecuteNextComboSecondaryAttack = true;
			}
		}
	}
	else if (Info.CurrentMovementState == EEixMovementState::Rolling
		&& Info.InputEvent == IE_Pressed
		&& State.bInRollingAttackWindow)
	{
		State.bWantsExecuteSecondaryAttackAfterRolling = true;
		if (State.bCanExecuteAttackWhileRolling)
		{
			ProceedRollingAttack();
		}
	}
}

void UEixCharacterMeleeCombatComp::SetInitialAttacks(FEixMeleeWeaponAttackConfig In_StartComboAttackConfig,
                                                     FEixMeleeWeaponAttackConfig In_OnRunAttackConfig,
                                                     FEixMeleeWeaponAttackConfig In_RightAfterRollAttackConfig)
{
	StartComboAttackConfig = In_StartComboAttackConfig;
	OnRunAttackConfig = In_OnRunAttackConfig;
	RightAfterRollAttackConfig = In_RightAfterRollAttackConfig;
}

void UEixCharacterMeleeCombatComp::OpenComboWindow(FEixMeleeWeaponAttackConfig In_NextComboAttackConfig)
{
	State.bInComboWindow = true;
	NextComboAttackConfig = In_NextComboAttackConfig;
}

void UEixCharacterMeleeCombatComp::CloseComboWindow()
{
	State.bInComboWindow = false;
	NextComboAttackConfig.PrimaryAttackMontage = nullptr;
	NextComboAttackConfig.SecondaryAttackMontage = nullptr;
}

void UEixCharacterMeleeCombatComp::OpenRollingAttackWindow()
{
	State.bInRollingAttackWindow = true;
}

void UEixCharacterMeleeCombatComp::ProceedCombo()
{
	State.bCanExecuteAttack = true;
	if (State.bWantsExecuteNextComboPrimaryAttack)
	{
		ExecuteAttack(GetPrimaryAttackMontage());
	}
	else if (State.bWantsExecuteNextComboSecondaryAttack)
	{
		ExecuteAttack(GetSecondaryAttackMontage());
	}
}

void UEixCharacterMeleeCombatComp::ProceedChargedAttack(UAnimMontage* KeepChargeMontage, UAnimMontage* ReleaseNowMontage)
{
	if (State.bSecondaryAttackReleased)
	{
		ExecuteAttack(ReleaseNowMontage);
	}
	else
	{
		ExecuteAttack(KeepChargeMontage);
	}
}

void UEixCharacterMeleeCombatComp::ProceedRollingAttack()
{
	if (State.bWantsExecutePrimaryAttackAfterRolling)
	{
		EixCharacterOwner->GetEixPlayerCharacterMovement()->StopRolling();
		ExecuteAttack(RightAfterRollAttackConfig.PrimaryAttackMontage);
	}
	else if (State.bWantsExecuteSecondaryAttackAfterRolling)
	{
		EixCharacterOwner->GetEixPlayerCharacterMovement()->StopRolling();
		ExecuteAttack(RightAfterRollAttackConfig.SecondaryAttackMontage);
	}
	else
	{
		State.bCanExecuteAttackWhileRolling = true;
	}
}

void UEixCharacterMeleeCombatComp::ReleaseChargedAttack(UAnimMontage* ReleaseChargedAttack)
{
	ExecuteAttack(ReleaseChargedAttack);
}

void UEixCharacterMeleeCombatComp::ComboAttackReleased()
{
	State.bCanHaltCombo = true;
}

void UEixCharacterMeleeCombatComp::HaltCombo(bool bForce /* = false */)
{
	if (State.bCanHaltCombo || bForce)
	{
		State.Reset();
		EixCharacterOwner->StopAnimMontage(CurrentPlayingAttackMontage);
	}
}

void UEixCharacterMeleeCombatComp::PerformPrimaryAttack()
{
	if (State.bCanExecuteAttack)
	{
		UAnimMontage* AttackMontage = GetPrimaryAttackMontage();
		if (IsValid(AttackMontage))
		{
			ExecuteAttack(AttackMontage);
		}
	}
	else if (State.bInComboWindow)
	{
		State.bWantsExecuteNextComboPrimaryAttack = true;
		State.bWantsExecuteNextComboSecondaryAttack = false;
	}
}

void UEixCharacterMeleeCombatComp::PerformSecondaryAttack()
{
	if (State.bCanExecuteAttack)
	{
		UAnimMontage* AttackMontage = GetSecondaryAttackMontage();
		if (IsValid(AttackMontage))
		{
			ExecuteAttack(AttackMontage);
		}
	}
	else if (State.bInComboWindow)
	{
		State.bWantsExecuteNextComboPrimaryAttack = false;
		State.bWantsExecuteNextComboSecondaryAttack = true;
	}
}

UAnimMontage* UEixCharacterMeleeCombatComp::GetPrimaryAttackMontage() const
{
	return State.bInComboWindow
		       ? NextComboAttackConfig.PrimaryAttackMontage
		       : StartComboAttackConfig.PrimaryAttackMontage;
}

UAnimMontage* UEixCharacterMeleeCombatComp::GetSecondaryAttackMontage() const
{
	return State.bInComboWindow
		       ? NextComboAttackConfig.SecondaryAttackMontage
		       : StartComboAttackConfig.SecondaryAttackMontage;
}

void UEixCharacterMeleeCombatComp::ExecuteAttack(UAnimMontage* AttackMontage)
{
	if (!IsValid(AttackMontage))
	{
		return;
	}
	State.bIsPlayingAttack = true;
	State.bCanHaltCombo = false;
	State.bCanExecuteAttack = false;
	State.bInComboWindow = false;
	State.bWantsExecuteNextComboPrimaryAttack = false;
	State.bWantsExecuteNextComboSecondaryAttack = false;
	State.bInRollingAttackWindow = false;
	State.bCanExecuteAttackWhileRolling = false;
	State.bWantsExecutePrimaryAttackAfterRolling = false;
	State.bWantsExecuteSecondaryAttackAfterRolling = false;
	CurrentPlayingAttackMontage = AttackMontage;
	EixCharacterOwner->PlayAnimMontage(AttackMontage);
}

UAnimInstance* UEixCharacterMeleeCombatComp::GetOwnerAnimInstance() const
{
	const USkeletalMeshComponent* Mesh = EixCharacterOwner->GetMesh();
	return Mesh ? Mesh->GetAnimInstance() : nullptr; 
}

void UEixCharacterMeleeCombatComp::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(CurrentPlayingAttackMontage))
	{
		return;
	}
	if (!bInterrupted && Montage->GetPrimaryAssetId() == CurrentPlayingAttackMontage->GetPrimaryAssetId())
	{
		State.Reset();
	}
}

void UEixCharacterMeleeCombatComp::OnMovementStateChanged(EEixMovementState PrevMovementState,
	EEixMovementState NewMovementState)
{
	if (PrevMovementState == EEixMovementState::Rolling && NewMovementState == EEixMovementState::OnGround)
	{
		State.bInRollingAttackWindow = false;
		State.bCanExecuteAttackWhileRolling = false;
	}
}
