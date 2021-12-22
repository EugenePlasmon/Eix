#include "Animations/EixPlayerCharacterAnimInstance.h"
#include "Characters/EixPlayerCharacter.h"

void UEixPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AEixPlayerCharacter>(), TEXT("UEixPlayerCharacterAnimInstance owner must be AEixPlayerCharacter"));
	EixCharacterOwner = StaticCast<AEixPlayerCharacter*>(TryGetPawnOwner());
}

void UEixPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
