#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RotationDuringRootMot.generated.h"

UCLASS()
class EIXCORE_API UAnimNotify_RotationDuringRootMot : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Eix")
	bool bEnable;
};
