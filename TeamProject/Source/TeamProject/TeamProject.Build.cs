// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeamProject : ModuleRules
{
<<<<<<< HEAD
    public TeamProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
=======
	public TeamProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
>>>>>>> upstream/main

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine",
            "InputCore", "EnhancedInput", "UMG",
            "MoviePlayer", "AIModule", "Paper2D", "RenderCore", "RHI", /*"Graphics",*/
        });

<<<<<<< HEAD
        PrivateDependencyModuleNames.AddRange(new string[] { });
=======
        PrivateDependencyModuleNames.AddRange(new string[] {  });
>>>>>>> upstream/main

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PublicIncludePaths.AddRange(new string[]
        {
            this.Name
        });

    }
}
