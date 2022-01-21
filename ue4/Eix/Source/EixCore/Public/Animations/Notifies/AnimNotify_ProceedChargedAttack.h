#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ProceedChargedAttack.generated.h"

UCLASS()
class EIXCORE_API UAnimNotify_ProceedChargedAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* KeepChargeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* ReleaseNowMontage;
};
