#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/Weapons/EixMeleeWeaponAttackConfig.h"
#include "EixCharacterMeleeCombatComp.generated.h"

class AEixPlayerCharacter;

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
	bool IsAttackExecuting() const { return bIsPlayingAttack && !bCanHaltCombo; }
	
	UFUNCTION(BlueprintGetter)
	bool IsAttackFading() const { return bIsPlayingAttack && bCanHaltCombo; }
	
	UFUNCTION(BlueprintGetter)
	bool IsPlayingAttack() const { return bIsPlayingAttack; }
	
	UFUNCTION(BlueprintGetter)
	bool CanHaltCombo() const { return bCanHaltCombo; }

	UFUNCTION(BlueprintCallable)
	void PrimaryAttack(EInputEvent InputEvent);

	UFUNCTION(BlueprintCallable)
	void SecondaryAttack(EInputEvent InputEvent);

	UFUNCTION(BlueprintCallable)
	void SetStartComboAttack(FEixMeleeWeaponAttackConfig In_StartComboAttackConfig);

	UFUNCTION(BlueprintCallable)
	void OpenComboWindow(FEixMeleeWeaponAttackConfig In_NextComboAttackConfig);

	UFUNCTION(BlueprintCallable)
	void CloseComboWindow();

	UFUNCTION(BlueprintCallable)
	void ProceedCombo();

	UFUNCTION(BlueprintCallable)
	void ProceedChargedAttack(UAnimMontage* KeepChargeMontage, UAnimMontage* ReleaseNowMontage);
	
	UFUNCTION(BlueprintCallable)
	void ReleaseChargedAttack(UAnimMontage* ReleaseChargedAttack);

	UFUNCTION(BlueprintCallable)
	void ComboAttackReleased();

	UFUNCTION(BlueprintCallable)
	void HaltCombo(bool bForce = false);

private:
	UPROPERTY()
	FEixMeleeWeaponAttackConfig StartComboAttackConfig;
	
	UPROPERTY()
	FEixMeleeWeaponAttackConfig NextComboAttackConfig;

	UPROPERTY()
	UAnimMontage* CurrentPlayingAttackMontage;
	
	bool bIsPlayingAttack = false;
	bool bCanHaltCombo = false;
	bool bCanExecuteAttack = true;
	bool bInComboWindow = false;
	bool bSecondaryAttackReleased = true;
	bool bWantsExecuteNextComboPrimaryAttack = false;
	bool bWantsExecuteNextComboSecondaryAttack = false;
	void ResetAttackState();

	TWeakObjectPtr<AEixPlayerCharacter> EixCharacterOwner;

	void PerformPrimaryAttack();
	void PerformSecondaryAttack();

	UAnimMontage* GetPrimaryAttackMontage() const;
	UAnimMontage* GetSecondaryAttackMontage() const;

	void ExecuteAttack(UAnimMontage* AttackMontage);

	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
