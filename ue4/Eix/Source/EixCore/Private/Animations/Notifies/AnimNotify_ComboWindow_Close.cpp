#include "Animations/Notifies/AnimNotify_ComboWindow_Close.h"
#include "Characters/EixPlayerCharacter.h"
#include "Components/EixCharacterMeleeCombatComp.h"

void UAnimNotify_ComboWindow_Close::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	const AEixPlayerCharacter* PlayerCharacter = Cast<AEixPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMeleeCombatComp()->CloseComboWindow();
	}
}
