#include "Components/EixCharacterHitReactionComp.h"
#include "Characters/EixCharacter.h"

UEixCharacterHitReactionComp::UEixCharacterHitReactionComp()
{
}

void UEixCharacterHitReactionComp::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA<AEixCharacter>());
	EixCharacterOwner = StaticCast<AEixCharacter*>(GetOwner());
	GetOwnerAnimInstance()->OnMontageEnded
	                      .AddDynamic(this, &UEixCharacterHitReactionComp::OnMontageEnded);
}

void UEixCharacterHitReactionComp::HandleHit(const FDamageEvent& DamageEvent)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		// TODO: Damage can be of different severity levels which means different hit reactions
		// TODO: Hit reaction can be stopped after certain stagger time.
		// TODO: Stagger time should be configured via data tables and depend on severity
		// TODO: Stagger time logic should be determined as in dark souls
		const FPointDamageEvent* PointDamageEvent = StaticCast<const FPointDamageEvent*>(&DamageEvent);
		const float Angle = FMath::FindDeltaAngleDegrees(PointDamageEvent->ShotDirection.Rotation().Yaw,
		                                                 EixCharacterOwner->GetActorRotation().Yaw);
		EixCharacterOwner->PlayAnimMontage(GetHitReactionMontage(Angle));
	}
}

UAnimInstance* UEixCharacterHitReactionComp::GetOwnerAnimInstance() const
{
	const USkeletalMeshComponent* Mesh = EixCharacterOwner->GetMesh();
	return Mesh ? Mesh->GetAnimInstance() : nullptr; 
}

UAnimMontage* UEixCharacterHitReactionComp::GetHitReactionMontage(float Angle) const
{
	if (Angle >= -45.f && Angle <= 45.f) // from the back
	{
		return HitReactionAnimConfig.FromTheBack;
	}
	if (Angle >= -135.f && Angle < 0.f) // from the left
	{
		return HitReactionAnimConfig.FromTheLeft;
	}
	if (Angle <= 135.f && Angle > 0.f) // from the right
	{
		return HitReactionAnimConfig.FromTheRight;
	}
	// possible angle value is in range of [-180, -135) U (135, 180]
	return HitReactionAnimConfig.FromTheFront;
}

void UEixCharacterHitReactionComp::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
}
