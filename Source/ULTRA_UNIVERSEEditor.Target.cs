// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ULTRA_UNIVERSEEditorTarget : TargetRules
{
	public ULTRA_UNIVERSEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ULTRA_UNIVERSE" } );
	}
}
