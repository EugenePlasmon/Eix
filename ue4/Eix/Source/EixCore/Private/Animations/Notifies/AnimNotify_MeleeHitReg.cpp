#include "Animations/Notifies/AnimNotify_MeleeHitReg.h"
#include "Actors/Weapons/EixMeleeWeapon.h"
#include "Characters/EixPlayerCharacter.h"

void UAnimNotify_MeleeHitReg::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMeleeWeapon()->SetHitRegistrationEnabled(bEnabled);
	}
}
