#include "Animations/Notifies/AnimNotify_RotationDuringRootMot.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_RotationDuringRootMot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (IsValid(Character))
	{
		Character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = bEnable;
	}
}
