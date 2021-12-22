#include "Components/EixCharacterIKComp.h"
#include "Subsystems/DebugSubsystem.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UEixCharacterIKComp::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("UEixCharacterIKComponent works only with ACharacter"));
	CharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

bool UEixCharacterIKComp::NeedCalculateIK() const
{
	if (!CharacterOwner.IsValid())
	{
		return false;
	}
	return CharacterOwner->GetCharacterMovement()->IsMovingOnGround();
}

void UEixCharacterIKComp::UpdateIK(float DeltaTime)
{
	const bool bResetIK = !NeedCalculateIK()
		|| LeftFootSocketName.IsNone()
		|| RightFootSocketName.IsNone();

	if (bResetIK)
	{
		TargetIKOffset_Pelvis = FVector(0.f);
		TargetIKOffset_LeftFoot = FVector(0.f);
		TargetIKOffset_RightFoot = FVector(0.f);
		TargetIKRotation_LeftFoot = FRotator(0.f);
		TargetIKRotation_RightFoot = FRotator(0.f);
	}
	else
	{
		FVector FloorOffset_LeftFoot, FloorOffset_RightFoot;
		GetIKFootOffsetAndRotationForSocket(LeftFootSocketName, FloorOffset_LeftFoot, TargetIKRotation_LeftFoot);
		GetIKFootOffsetAndRotationForSocket(RightFootSocketName, FloorOffset_RightFoot, TargetIKRotation_RightFoot);
		if (FloorOffset_LeftFoot.Z < FloorOffset_RightFoot.Z)
		{
			TargetIKOffset_Pelvis = FloorOffset_LeftFoot;
		}
		else
		{
			TargetIKOffset_Pelvis = FloorOffset_RightFoot;
		}
		TargetIKOffset_LeftFoot = FloorOffset_LeftFoot - TargetIKOffset_Pelvis;
		TargetIKOffset_RightFoot = FloorOffset_RightFoot - TargetIKOffset_Pelvis;

		if (bDisableIKFootRotationWhileMoving)
		{
			const float CharacterAccelerationSqr = CharacterOwner->GetCharacterMovement()->GetCurrentAcceleration().SizeSquared();
			if (CharacterAccelerationSqr > SMALL_NUMBER)
			{
				TargetIKRotation_LeftFoot = FRotator(0.f);
				TargetIKRotation_RightFoot = FRotator(0.f);
			}
		}
	}

	IKOffset_Pelvis = FMath::VInterpTo(IKOffset_Pelvis, TargetIKOffset_Pelvis, DeltaTime, IKOffsetInterpSpeed);
	IKOffset_LeftFoot = FMath::VInterpTo(IKOffset_LeftFoot, TargetIKOffset_LeftFoot, DeltaTime, IKOffsetInterpSpeed);
	IKOffset_RightFoot = FMath::VInterpTo(IKOffset_RightFoot, TargetIKOffset_RightFoot, DeltaTime, IKOffsetInterpSpeed);
	IKRotation_LeftFoot = FMath::RInterpTo(IKRotation_LeftFoot, TargetIKRotation_LeftFoot, DeltaTime, IKRotationInterpSpeed);
	IKRotation_RightFoot = FMath::RInterpTo(IKRotation_RightFoot, TargetIKRotation_RightFoot, DeltaTime, IKRotationInterpSpeed);
}

void UEixCharacterIKComp::GetIKFootOffsetAndRotationForSocket(const FName& SocketName, FVector& OutOffset, FRotator& OutRotation) const
{
	FHitResult Hit;
	const FVector RootLocation = CharacterOwner->GetMesh()->GetSocketLocation(RootBoneName);
	const FVector SocketLocation = CharacterOwner->GetMesh()->GetSocketLocation(SocketName);
	const FVector TraceFloorLocation = FVector(SocketLocation.X, SocketLocation.Y, RootLocation.Z);
	const FVector TraceStart = TraceFloorLocation + FVector(0.f, 0.f, TraceHalfLength);
	const FVector TraceEnd = TraceFloorLocation - FVector(0.f, 0.f, TraceHalfLength);
	const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	const bool bDrawDebug = GetWorld()->GetGameInstance()->GetSubsystem<UDebugSubsystem>()
	                                  ->IsDebugCategoryEnabled(DebugCategory::IKCharacter);
	const EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	const bool bGotHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd,
	                                                           TraceTypeQuery, true, TArray<AActor*>(),
	                                                           DrawDebugTrace, Hit, true);
	if (bGotHit && CharacterOwner->GetCharacterMovement()->IsWalkable(Hit))
	{
		const FVector FootInitialLocation = TraceFloorLocation + FootHeight * FVector::UpVector;
		const FVector FootTargetLocation = Hit.ImpactPoint + Hit.ImpactNormal * FootHeight;
		OutOffset = FootTargetLocation - FootInitialLocation;

		const float PitchRadians = -FMath::Atan2(Hit.ImpactNormal.X, Hit.ImpactNormal.Z);
		const float RollRadians = FMath::Atan2(Hit.ImpactNormal.Y, Hit.ImpactNormal.Z);
		OutRotation = FRotator(FMath::RadiansToDegrees(PitchRadians), 0.f, FMath::RadiansToDegrees(RollRadians));
	}
	else
	{
		OutOffset = FVector(0.f);
		OutRotation = FRotator(0.f);
	}
}
