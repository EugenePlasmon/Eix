#include "Characters/EixPlayerCameraManager.h"

#include "Characters/EixPlayerCharacter.h"

AEixPlayerCameraManager::AEixPlayerCameraManager()
{
	ViewPitchMin = -80.f;
	ViewPitchMax = 30.f;
}

void AEixPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	if (!IsValid(OutVT.Target))
	{
		return;
	}
	checkf(OutVT.Target->IsA<AEixPlayerCharacter>(), TEXT("AEixPlayerCameraManager can only work with AEixPlayerCharacter"));

	//TODO: Custom camera implementation. Use commented code below as a template
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);

	//FVector OutLocation;
	//FRotator OutRotation;
	//float OutFOV;
	//if (BlueprintUpdateCamera(OutVT.Target, OutLocation, OutRotation, OutFOV))
	//{
	//	OutVT.POV.Location = OutLocation;
	//	OutVT.POV.Rotation = OutRotation;
	//	OutVT.POV.FOV = OutFOV;
	//}
	//else
	//{
	//	OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
	//}
}
