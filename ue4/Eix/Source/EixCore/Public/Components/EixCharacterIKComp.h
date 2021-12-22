#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EixCharacterIKComp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixCharacterIKComp : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void UpdateIK(float DeltaTime);

	FVector FORCEINLINE GetIKOffset_Pelvis() const { return IKOffset_Pelvis; }
	FVector FORCEINLINE GetIKOffset_LeftFoot() const { return IKOffset_LeftFoot; }
	FVector FORCEINLINE GetIKOffset_RightFoot() const { return IKOffset_RightFoot; }
	FRotator FORCEINLINE GetIKRotation_LeftFoot() const { return IKRotation_LeftFoot; }
	FRotator FORCEINLINE GetIKRotation_RightFoot() const { return IKRotation_RightFoot; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "IK|Names")
	FName RootBoneName = FName("root");

	UPROPERTY(EditDefaultsOnly, Category = "IK|Names")
	FName LeftFootSocketName = FName("ik_foot_l_socket");

	UPROPERTY(EditDefaultsOnly, Category = "IK|Names")
	FName RightFootSocketName = FName("ik_foot_r_socket");

	UPROPERTY(EditDefaultsOnly, Category = "IK")
	bool bDisableIKFootRotationWhileMoving = true;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
	float TraceHalfLength = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
	float IKOffsetInterpSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
	float IKRotationInterpSpeed = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
	float FootHeight = 13.5f;

private:
	TWeakObjectPtr<class ACharacter> CharacterOwner;

	// Interpolated offsets/rotations
	FVector IKOffset_Pelvis;
	FVector IKOffset_LeftFoot;
	FVector IKOffset_RightFoot;
	FRotator IKRotation_LeftFoot;
	FRotator IKRotation_RightFoot;

	// Target offsets/rotations
	FVector TargetIKOffset_Pelvis;
	FVector TargetIKOffset_LeftFoot;
	FVector TargetIKOffset_RightFoot;
	FRotator TargetIKRotation_LeftFoot;
	FRotator TargetIKRotation_RightFoot;

	bool NeedCalculateIK() const;

	void GetIKFootOffsetAndRotationForSocket(const FName& SocketName, OUT FVector& OutOffset, OUT FRotator& OutRotation) const;
};
