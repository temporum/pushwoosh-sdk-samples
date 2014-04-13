using System;
using MonoTouch.ObjCRuntime;

[assembly: LinkWith ("libXamarinPWNoRuntime.a", LinkTarget.Arm64 | LinkTarget.ArmV7 | LinkTarget.ArmV7s | LinkTarget.Simulator, ForceLoad = true)]
