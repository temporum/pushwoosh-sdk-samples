using System;
using MonoTouch.ObjCRuntime;

[assembly: LinkWith ("PushNotificationManager.a", LinkTarget.Arm64 | LinkTarget.ArmV7 | LinkTarget.ArmV7s | LinkTarget.Simulator, ForceLoad = true, Frameworks = "CoreGraphics CoreFoundation Security StoreKit")]
