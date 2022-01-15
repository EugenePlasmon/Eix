#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	bool IsAttackExecuting() const { return bIsPlayingAttack && !bCanHaltCombo; }
	bool IsAttackFading() const { return bIsPlayingAttack && bCanHaltCombo; }
	bool IsPlayingAttack() const { return bIsPlayingAttack; } 
	bool CanHaltCombo() const { return bCanHaltCombo; }

	void PrimaryAttack();
	void SecondaryAttack();
	
	void SetNextComboAttacks(FName In_NextComboPrimaryAttack, FName In_NextComboSecondaryAttack);
	void ProceedCombo();
	void ComboAttackReleased();
	void HaltCombo(bool bForce = false);

protected:
	// TODO: to be replaced with weapon specific data
	UPROPERTY(EditDefaultsOnly, Category="Eix")
	UAnimMontage* ComboAnimMontage;

private:
	FName NextComboPrimaryAttack = NAME_None;
	FName NextComboSecondaryAttack = NAME_None;
	bool bIsPlayingAttack = false;
	bool bCanHaltCombo = false;
	bool bCanExecuteAttack = true;
	bool bWantsExecuteNextComboPrimaryAttack = false;
	bool bWantsExecuteNextComboSecondaryAttack = false;
	void ResetAttackState();

	FTimerHandle PlayingAttackTimer;

	TWeakObjectPtr<AEixPlayerCharacter> EixCharacterOwner;

	void ExecuteAttack(const FName& MontageSection);

	float PlayAnimMontage(UAnimMontage* AnimMontage, FName Section) const;
	UAnimInstance* GetOwnerAnimInstance() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
