#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/Weapons/EixMeleeAttackPerformingInfo.h"
#include "Types/Weapons/EixMeleeWeaponAttackConfig.h"
#include "EixCharacterMeleeCombatComp.generated.h"

class AEixPlayerCharacter;

USTRUCT()
struct FEixMeleeAttackState
{
	GENERATED_BODY()
	
	uint8 bCanExecuteAttack:1;
	uint8 bIsPlayingAttack:1;
	uint8 bSecondaryAttackReleased:1;
	
	uint8 bInComboWindow:1;
	uint8 bCanHaltCombo:1;
	uint8 bWantsExecuteNextComboPrimaryAttack:1;
	uint8 bWantsExecuteNextComboSecondaryAttack:1;
	
	uint8 bInRollingAttackWindow:1;
	uint8 bCanExecuteAttackWhileRolling:1;
	uint8 bWantsExecutePrimaryAttackAfterRolling:1;
	uint8 bWantsExecuteSecondaryAttackAfterRolling:1;

	FEixMeleeAttackState()
	{
		Reset();
	}
	
	void Reset()
	{
		bCanExecuteAttack = true;
		bIsPlayingAttack = false;
		bSecondaryAttackReleased = true;
		
		bInComboWindow = false;
		bCanHaltCombo = false;
		bWantsExecuteNextComboPrimaryAttack = false;
		bWantsExecuteNextComboSecondaryAttack = false;
		
		bInRollingAttackWindow = false;
		bCanExecuteAttackWhileRolling = false;
		bWantsExecutePrimaryAttackAfterRolling = false;
		bWantsExecuteSecondaryAttackAfterRolling = false;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EIXCORE_API UEixCharacterMeleeCombatComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEixCharacterMeleeCombatComp();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;
#pragma endregion

public:
	// TODO: revisit this naming
	UFUNCTION(BlueprintGetter)
	bool IsAttackExecuting() const { return State.bIsPlayingAttack && !State.bCanHaltCombo; }
	
	UFUNCTION(BlueprintGetter)
	bool IsAttackFading() const { return State.bIsPlayingAttack && State.bCanHaltCombo; }
	
	UFUNCTION(BlueprintGetter)
	bool IsPlayingAttack() const { return State.bIsPlayingAttack; }
	
	UFUNCTION(BlueprintGetter)
	bool CanHaltCombo() const { return State.bCanHaltCombo; }

	UFUNCTION(BlueprintCallable)
	void PrimaryAttack(const FEixMeleeAttackPerformingInfo& Info);

	UFUNCTION(BlueprintCallable)
	void SecondaryAttack(const FEixMeleeAttackPerformingInfo& Info);

	UFUNCTION(BlueprintCallable)
	void SetInitialAttacks(FEixMeleeWeaponAttackConfig In_StartComboAttackConfig,
	                       FEixMeleeWeaponAttackConfig In_OnRunAttackConfig,
	                       FEixMeleeWeaponAttackConfig In_RightAfterRollAttackConfig);

	UFUNCTION(BlueprintCallable)
	void OpenComboWindow(FEixMeleeWeaponAttackConfig In_NextComboAttackConfig);

	UFUNCTION(BlueprintCallable)
	void CloseComboWindow();

	UFUNCTION(BlueprintCallable)
	void OpenRollingAttackWindow();

	UFUNCTION(BlueprintCallable)
	void ProceedCombo();

	UFUNCTION(BlueprintCallable)
	void ProceedChargedAttack(UAnimMontage* KeepChargeMontage, UAnimMontage* ReleaseNowMontage);

	UFUNCTION(BlueprintCallable)
	void ProceedRollingAttack();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseChargedAttack(UAnimMontage* ReleaseChargedAttack);

	UFUNCTION(BlueprintCallable)
	void ComboAttackReleased();

	UFUNCTION(BlueprintCallable)
	void HaltCombo(bool bForce = false);

private:
	FEixMeleeAttackState State;
	
	FEixMeleeWeaponAttackConfig StartComboAttackConfig;
	FEixMeleeWeaponAttackConfig NextComboAttackConfig;
	FEixMeleeWeaponAttackConfig OnRunAttackConfig;
	FEixMeleeWeaponAttackConfig RightAfterRollAttackConfig;
	
	UPROPERTY()
	UAnimMontage* CurrentPlayingAttackMontage;

	TWeakObjectPtr<AEixPlayerCharacter> EixCharacterOwner;

	void PerformPrimaryAttack();
	void PerformSecondaryAttack();

	UAnimMontage* GetPrimaryAttackMontage() const;
	UAnimMontage* GetSecondaryAttackMontage() const;

	void ExecuteAttack(UAnimMontage* AttackMontage);

	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMovementStateChanged(EEixMovementState PrevMovementState, EEixMovementState NewMovementState);
};
