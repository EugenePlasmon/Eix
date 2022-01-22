#include "Animations/Notifies/AnimNotify_ProceedChargedAttack.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/EixCharacterMeleeCombatComp.h"

void UAnimNotify_ProceedChargedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMeleeCombatComp()->ProceedChargedAttack(KeepChargeMontage, ReleaseNowMontage);
	}
}
