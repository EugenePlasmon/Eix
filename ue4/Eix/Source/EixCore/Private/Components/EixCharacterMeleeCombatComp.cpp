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

void UEixCharacterMeleeCombatComp::PrimaryAttack()
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

void UEixCharacterMeleeCombatComp::SecondaryAttack()
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
		PrimaryAttack();
	}
	else if (bWantsExecuteNextComboSecondaryAttack)
	{
		SecondaryAttack();
	}
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
	bIsPlayingAttack = false;
	bCanHaltCombo = false;
	bCanExecuteAttack = true;
	bInComboWindow = false;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
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
	if (!bInterrupted && Montage->GetPrimaryAssetId() == CurrentPlayingAttackMontage->GetPrimaryAssetId())
	{
		ResetAttackState();
	}
}
