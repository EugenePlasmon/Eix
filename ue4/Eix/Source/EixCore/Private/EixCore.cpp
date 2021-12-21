#include "EixCore.h"
#include "Log.h"

#include "Modules/ModuleManager.h"

void FEixCore::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("EixCore module starting up"));
}

void FEixCore::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("EixCore module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, EixCore, "EixCore");