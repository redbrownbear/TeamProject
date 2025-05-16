// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeamProject : ModuleRules
{
	public TeamProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine",
            "InputCore", "EnhancedInput", "UMG",
            "MoviePlayer", "AIModule", "Paper2D", "RenderCore", "RHI", /*"Graphics",*/
        });

<<<<<<< HEAD
        PublicIncludePaths.AddRange(new string[]
        {
            this.Name
        });
=======
        PrivateDependencyModuleNames.AddRange(new string[] {  });
>>>>>>> origin/main

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
<<<<<<< HEAD
=======

        PublicIncludePaths.AddRange(new string[]
        {
            this.Name
        });

>>>>>>> origin/main
    }
}
