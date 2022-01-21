#include "Animations/Notifies/AnimNotify_ReleaseChargedAttack.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/EixCharacterMeleeCombatComp.h"

void UAnimNotify_ReleaseChargedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMeleeCombatComp()->ReleaseChargedAttack(ReleaseAttackMontage);
	}	
}
