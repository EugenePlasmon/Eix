#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ComboWindow.generated.h"

UCLASS()
class EIXCORE_API UAnimNotify_ComboWindow : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Eix")
	FName NextComboPrimaryAttack;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Eix")
	FName NextComboSecondaryAttack;
};
