#pragma once

#include "CoreMinimal.h"
#include "Animations/EixCharacterAnimInstance.h"
#include "EixPlayerCharacterAnimInstance.generated.h"

UCLASS()
class EIXCORE_API UEixPlayerCharacterAnimInstance : public UEixCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma region Misc
private:
	TWeakObjectPtr<class AEixPlayerCharacter> EixCharacterOwner;
#pragma endregion
};
