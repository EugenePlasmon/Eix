#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ReleaseChargedAttack.generated.h"

UCLASS()
class EIXCORE_API UAnimNotify_ReleaseChargedAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* ReleaseAttackMontage;
};
