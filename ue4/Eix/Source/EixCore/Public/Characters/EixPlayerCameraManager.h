#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "EixPlayerCameraManager.Generated.h"

UCLASS()
class AEixPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AEixPlayerCameraManager();

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
};