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
		if (bCanExecuteAttack)
		{
			const bool bOnRun = !Info.CurrentInputAcceleration.IsNearlyZero() && Info.CurrentGait == EEixGait::Sprint;
			UAnimMontage* AttackMontage = bOnRun ? OnRunAttackConfig.PrimaryAttackMontage : GetPrimaryAttackMontage();
			ExecuteAttack(AttackMontage);
		}
		else if (bInComboWindow)
		{
			bWantsExecuteNextComboPrimaryAttack = true;
			bWantsExecuteNextComboSecondaryAttack = false;
		}
	}
	else if (Info.CurrentMovementState == EEixMovementState::Rolling
		&& Info.InputEvent == IE_Pressed
		&& bInRollingAttackWindow)
	{
		bWantsExecutePrimaryAttackAfterRolling = true;
	}
}

void UEixCharacterMeleeCombatComp::SecondaryAttack(const FEixMeleeAttackPerformingInfo& Info)
{
	if (Info.CurrentMovementState == EEixMovementState::OnGround)
	{
		bSecondaryAttackReleased = Info.InputEvent == IE_Released;
		
		if (Info.InputEvent == IE_Pressed)
		{
			if (bCanExecuteAttack)
			{
				const bool bOnRun = !Info.CurrentInputAcceleration.IsNearlyZero() && Info.CurrentGait == EEixGait::Sprint;
				UAnimMontage* AttackMontage = bOnRun ? OnRunAttackConfig.SecondaryAttackMontage : GetSecondaryAttackMontage();
				ExecuteAttack(AttackMontage);
			}
			else if (bInComboWindow)
			{
				bWantsExecuteNextComboPrimaryAttack = false;
				bWantsExecuteNextComboSecondaryAttack = true;
			}
		}
	}
	else if (Info.CurrentMovementState == EEixMovementState::Rolling
		&& Info.InputEvent == IE_Pressed
		&& bInRollingAttackWindow)
	{
		bWantsExecuteSecondaryAttackAfterRolling = true;
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
	bInComboWindow = true;
	NextComboAttackConfig = In_NextComboAttackConfig;
}

void UEixCharacterMeleeCombatComp::CloseComboWindow()
{
	bInComboWindow = false;
	NextComboAttackConfig.PrimaryAttackMontage = nullptr;
	NextComboAttackConfig.SecondaryAttackMontage = nullptr;
}

void UEixCharacterMeleeCombatComp::OpenRollingAttackWindow()
{
	bInRollingAttackWindow = true;
}

void UEixCharacterMeleeCombatComp::ProceedCombo()
{
	bCanExecuteAttack = true;
	if (bWantsExecuteNextComboPrimaryAttack)
	{
		ExecuteAttack(GetPrimaryAttackMontage());
	}
	else if (bWantsExecuteNextComboSecondaryAttack)
	{
		ExecuteAttack(GetSecondaryAttackMontage());
	}
}

void UEixCharacterMeleeCombatComp::ProceedChargedAttack(UAnimMontage* KeepChargeMontage, UAnimMontage* ReleaseNowMontage)
{
	if (bSecondaryAttackReleased)
	{
		ExecuteAttack(ReleaseNowMontage);
	}
	else
	{
		ExecuteAttack(KeepChargeMontage);
	}
}

void UEixCharacterMeleeCombatComp::ReleaseChargedAttack(UAnimMontage* ReleaseChargedAttack)
{
	ExecuteAttack(ReleaseChargedAttack);
}

void UEixCharacterMeleeCombatComp::ComboAttackReleased()
{
	bCanHaltCombo = true;
}

void UEixCharacterMeleeCombatComp::HaltCombo(bool bForce /* = false */)
{
	if (bCanHaltCombo || bForce)
	{
		ResetAttackState();
		EixCharacterOwner->StopAnimMontage(CurrentPlayingAttackMontage);
	}
}

void UEixCharacterMeleeCombatComp::ResetAttackState()
{
	NextComboAttackConfig.PrimaryAttackMontage = nullptr;
	NextComboAttackConfig.SecondaryAttackMontage = nullptr;
	CurrentPlayingAttackMontage = nullptr;
	bIsPlayingAttack = false;
	bCanHaltCombo = false;
	bCanExecuteAttack = true;
	bInComboWindow = false;
	bInRollingAttackWindow = false;
	bSecondaryAttackReleased = true;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
	bWantsExecutePrimaryAttackAfterRolling = false;
	bWantsExecuteSecondaryAttackAfterRolling = false;
}

void UEixCharacterMeleeCombatComp::PerformPrimaryAttack()
{
	if (bCanExecuteAttack)
	{
		UAnimMontage* AttackMontage = GetPrimaryAttackMontage();
		if (IsValid(AttackMontage))
		{
			ExecuteAttack(AttackMontage);
		}
	}
	else if (bInComboWindow)
	{
		bWantsExecuteNextComboPrimaryAttack = true;
		bWantsExecuteNextComboSecondaryAttack = false;
	}
}

void UEixCharacterMeleeCombatComp::PerformSecondaryAttack()
{
	if (bCanExecuteAttack)
	{
		UAnimMontage* AttackMontage = GetSecondaryAttackMontage();
		if (IsValid(AttackMontage))
		{
			ExecuteAttack(AttackMontage);
		}
	}
	else if (bInComboWindow)
	{
		bWantsExecuteNextComboPrimaryAttack = false;
		bWantsExecuteNextComboSecondaryAttack = true;
	}
}

UAnimMontage* UEixCharacterMeleeCombatComp::GetPrimaryAttackMontage() const
{
	return bInComboWindow
		       ? NextComboAttackConfig.PrimaryAttackMontage
		       : StartComboAttackConfig.PrimaryAttackMontage;
}

UAnimMontage* UEixCharacterMeleeCombatComp::GetSecondaryAttackMontage() const
{
	return bInComboWindow
		       ? NextComboAttackConfig.SecondaryAttackMontage
		       : StartComboAttackConfig.SecondaryAttackMontage;
}

void UEixCharacterMeleeCombatComp::ExecuteAttack(UAnimMontage* AttackMontage)
{
	if (!IsValid(AttackMontage))
	{
		return;
	}
	bIsPlayingAttack = true;
	bCanHaltCombo = false;
	bCanExecuteAttack = false;
	bInComboWindow = false;
	bInRollingAttackWindow = false;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
	bWantsExecutePrimaryAttackAfterRolling = false;
	bWantsExecuteSecondaryAttackAfterRolling = false;
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
		ResetAttackState();
	}
}

void UEixCharacterMeleeCombatComp::OnMovementStateChanged(EEixMovementState PrevMovementState,
	EEixMovementState NewMovementState)
{
	if (PrevMovementState == EEixMovementState::Rolling && NewMovementState == EEixMovementState::OnGround)
	{
		bInRollingAttackWindow = false;
		if (bWantsExecutePrimaryAttackAfterRolling)
		{
			ExecuteAttack(RightAfterRollAttackConfig.PrimaryAttackMontage);
		}
		else if (bWantsExecuteSecondaryAttackAfterRolling)
		{
			ExecuteAttack(RightAfterRollAttackConfig.SecondaryAttackMontage);
		}
	}
}
