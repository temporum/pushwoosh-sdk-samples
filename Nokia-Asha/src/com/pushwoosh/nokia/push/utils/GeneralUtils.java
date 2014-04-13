//
//  GeneralUtils.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed

package com.pushwoosh.nokia.push.utils;

/**
 * Date: 16.08.12
 * Time: 21:01
 *
 * @author mig35
 */
public class GeneralUtils
{
	public static String getDeviceUUID()
	{
		return System.getProperty("com.nokia.mid.imei");
	}

	public static void checkNotNullOrEmpty(String reference, String name)
	{
		checkNotNull(reference, name);
		if (reference.length() == 0)
		{
			throw new IllegalArgumentException(
					FormatUtils.format("Please set the %s constant and recompile the app.", new String[]{name}));				
		}
	}

	public static void checkNotNull(Object reference, String name)
	{
		if (reference == null)
		{
			throw new IllegalArgumentException(
					FormatUtils.format("Please set the %s constant and recompile the app.", new String[]{name}));
		}
	}
}
