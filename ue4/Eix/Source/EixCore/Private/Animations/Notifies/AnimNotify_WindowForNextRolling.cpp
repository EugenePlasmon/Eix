#include "Animations/Notifies/AnimNotify_WindowForNextRolling.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/Movement/EixPlayerCharacterMovComp.h"

void UAnimNotify_WindowForNextRolling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetEixPlayerCharacterMovement()->OpenWindowForNextRolling();
	}
}
