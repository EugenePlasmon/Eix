#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Types/Weapons/EixMeleeWeaponAttackConfig.h"
#include "AnimNotify_ComboWindow_Open.generated.h"

UCLASS()
class EIXCORE_API UAnimNotify_ComboWindow_Open : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eix")
	FEixMeleeWeaponAttackConfig NextComboAttackConfig;
};
