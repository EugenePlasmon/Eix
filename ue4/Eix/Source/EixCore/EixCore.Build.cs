using UnrealBuildTool;

public class EixCore : ModuleRules
{
    public EixCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;

        PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine" } );
        PrivateDependencyModuleNames.AddRange( new string[] { } );
    }
}