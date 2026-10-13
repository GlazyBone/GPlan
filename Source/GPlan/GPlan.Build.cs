//Copyright GPlan Project. All Rights Reserved.

using UnrealBuildTool;

public class GPlan : ModuleRules
{
	public GPlan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });

        //允许项目内所有子文件夹直接引用根目录的头文件
        PublicIncludePaths.Add(ModuleDirectory);
    }
}
