#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EixCharacterHitReactionComp.generated.h"

USTRUCT(BlueprintType)
struct FEixHitReactionAnimConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FromTheFront = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FromTheLeft = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FromTheRight = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FromTheBack = nullptr;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EIXCORE_API UEixCharacterHitReactionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEixCharacterHitReactionComp();

protected:
	virtual void BeginPlay() override;

public:
	void HandleHit(const FDamageEvent& DamageEvent);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix")
	FEixHitReactionAnimConfig HitReactionAnimConfig;
	
private:
	TWeakObjectPtr<class AEixCharacter> EixCharacterOwner;
	
	UAnimInstance* GetOwnerAnimInstance() const;

	UAnimMontage* GetHitReactionMontage(float Angle) const;
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
