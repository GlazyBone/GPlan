//Copyright GPlan Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GPlanTarget : TargetRules
{
	public GPlanTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("GPlan");
	}
}
