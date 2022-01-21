#include "Components/EixCharacterMeleeCombatComp.h"
#include "Characters/EixPlayerCharacter.h"

UEixCharacterMeleeCombatComp::UEixCharacterMeleeCombatComp()
{
}

void UEixCharacterMeleeCombatComp::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA<AEixPlayerCharacter>());
	EixCharacterOwner = StaticCast<AEixPlayerCharacter*>(GetOwner());
	GetOwnerAnimInstance()->OnMontageEnded.AddDynamic(this, &UEixCharacterMeleeCombatComp::OnMontageEnded);
}

void UEixCharacterMeleeCombatComp::PrimaryAttack(EInputEvent InputEvent)
{
	if (InputEvent == IE_Pressed)
	{
		PerformPrimaryAttack();
	}
}

void UEixCharacterMeleeCombatComp::SecondaryAttack(EInputEvent InputEvent)
{
	bSecondaryAttackReleased = InputEvent == IE_Released;
	
	if (InputEvent == IE_Pressed)
	{
		PerformSecondaryAttack();
	}
}

void UEixCharacterMeleeCombatComp::SetStartComboAttack(FEixMeleeWeaponAttackConfig In_StartComboAttackConfig)
{
	StartComboAttackConfig = In_StartComboAttackConfig;
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

void UEixCharacterMeleeCombatComp::ProceedCombo()
{
	bCanExecuteAttack = true;
	if (bWantsExecuteNextComboPrimaryAttack)
	{
		PerformPrimaryAttack();
	}
	else if (bWantsExecuteNextComboSecondaryAttack)
	{
		PerformSecondaryAttack();
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
	bSecondaryAttackReleased = true;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
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
	if (IsValid(NextComboAttackConfig.PrimaryAttackMontage))
	{
		return NextComboAttackConfig.PrimaryAttackMontage;
	}
	if (IsValid(StartComboAttackConfig.PrimaryAttackMontage))
	{
		return StartComboAttackConfig.PrimaryAttackMontage;
	}
	return nullptr;
}

UAnimMontage* UEixCharacterMeleeCombatComp::GetSecondaryAttackMontage() const
{
	if (IsValid(NextComboAttackConfig.SecondaryAttackMontage))
	{
		return NextComboAttackConfig.SecondaryAttackMontage;
	}
	if (IsValid(StartComboAttackConfig.SecondaryAttackMontage))
	{
		return StartComboAttackConfig.SecondaryAttackMontage;
	}
	return nullptr;
}

void UEixCharacterMeleeCombatComp::ExecuteAttack(UAnimMontage* AttackMontage)
{
	bIsPlayingAttack = true;
	bCanHaltCombo = false;
	bCanExecuteAttack = false;
	bInComboWindow = false;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
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
