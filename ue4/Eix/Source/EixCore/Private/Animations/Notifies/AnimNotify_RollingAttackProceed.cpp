#include "Animations/Notifies/AnimNotify_RollingAttackProceed.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/EixCharacterMeleeCombatComp.h"

void UAnimNotify_RollingAttackProceed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMeleeCombatComp()->ProceedRollingAttack();
	}
}
