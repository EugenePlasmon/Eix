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
	check(ComboAnimMontage);

	const bool bInComboWindow = !NextComboPrimaryAttack.IsNone();
	if (bCanExecuteAttack)
	{
		ExecuteAttack(NextComboPrimaryAttack);
	}
	else if (bInComboWindow)
	{
		bWantsExecuteNextComboPrimaryAttack = true;
		bWantsExecuteNextComboSecondaryAttack = false;
	}
}

void UEixCharacterMeleeCombatComp::SecondaryAttack()
{
	check(ComboAnimMontage);
	
	const bool bInComboWindow = !NextComboSecondaryAttack.IsNone();
	if (bCanExecuteAttack)
	{
		ExecuteAttack(NextComboSecondaryAttack);
	}
	else if (bInComboWindow)
	{
		bWantsExecuteNextComboPrimaryAttack = false;
		bWantsExecuteNextComboSecondaryAttack = true;
	}
}

void UEixCharacterMeleeCombatComp::SetNextComboAttacks(FName In_NextComboPrimaryAttack, FName In_NextComboSecondaryAttack)
{
	NextComboPrimaryAttack = In_NextComboPrimaryAttack;
	NextComboSecondaryAttack = In_NextComboSecondaryAttack;
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
		EixCharacterOwner->StopAnimMontage(ComboAnimMontage);
	}
}

void UEixCharacterMeleeCombatComp::ResetAttackState()
{
	NextComboPrimaryAttack = NAME_None;
	NextComboSecondaryAttack = NAME_None;
	bIsPlayingAttack = false;
	bCanHaltCombo = false;
	bCanExecuteAttack = true;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;
}

void UEixCharacterMeleeCombatComp::ExecuteAttack(const FName& MontageSection)
{
	bIsPlayingAttack = true;
	bCanHaltCombo = false;
	bCanExecuteAttack = false;
	bWantsExecuteNextComboPrimaryAttack = false;
	bWantsExecuteNextComboSecondaryAttack = false;

	const float Duration =
		PlayAnimMontage(ComboAnimMontage, MontageSection);
	
	// TODO: using timers with montage duration is wrong. Try OnMontageEnded
	GetWorld()->GetTimerManager().ClearTimer(PlayingAttackTimer);
	GetWorld()->GetTimerManager().SetTimer(PlayingAttackTimer, FTimerDelegate::CreateLambda([this]
	{
		this->bIsPlayingAttack = false;
	}), Duration, false);
}

float UEixCharacterMeleeCombatComp::PlayAnimMontage(UAnimMontage* AnimMontage, FName Section) const
{
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!AnimInstance)
	{
		return 0.f;
	}
	
	const float Duration = AnimInstance->Montage_Play(AnimMontage, 1.f,
	                                                  EMontagePlayReturnType::Duration);
	if (Duration > 0.f && Section != NAME_None)
	{
		AnimInstance->Montage_JumpToSection(Section, AnimMontage);
	}
	return Duration;
}

UAnimInstance* UEixCharacterMeleeCombatComp::GetOwnerAnimInstance() const
{
	const USkeletalMeshComponent* Mesh = EixCharacterOwner->GetMesh();
	return Mesh ? Mesh->GetAnimInstance() : nullptr; 
}

void UEixCharacterMeleeCombatComp::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && Montage->GetPrimaryAssetId() == ComboAnimMontage->GetPrimaryAssetId())
	{
		ResetAttackState();
	}
}
