#pragma once

#include "CoreMinimal.h"
#include "EixMeleeWeaponAttackConfig.Generated.h"

USTRUCT(BlueprintType)
struct FEixMeleeWeaponAttackConfig
{
	GENERATED_BODY()

	FEixMeleeWeaponAttackConfig(): PrimaryAttackMontage(nullptr), SecondaryAttackMontage(nullptr)
	{
	}

	FEixMeleeWeaponAttackConfig(UAnimMontage* PrimaryAttackMontage, UAnimMontage* SecondaryAttackMontage)
		: PrimaryAttackMontage(PrimaryAttackMontage), SecondaryAttackMontage(SecondaryAttackMontage)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* PrimaryAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SecondaryAttackMontage;
};