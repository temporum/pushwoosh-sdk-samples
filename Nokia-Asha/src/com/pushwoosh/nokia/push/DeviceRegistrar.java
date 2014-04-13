//
//  DeviceRegistrar.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed

package com.pushwoosh.nokia.push;

import com.pushwoosh.nokia.push.request.RegisterDeviceRequest;
import com.pushwoosh.nokia.push.request.RequestManager;
import com.pushwoosh.nokia.push.request.UnregisterDeviceRequest;

/**
 * Register/unregister with the App server.
 */
public class DeviceRegistrar
{
	static void registerWithServer(String deviceRegistrationID) throws Exception
	{
		System.out.println("Registering for pushes");
		
		RegisterDeviceRequest request = new RegisterDeviceRequest(deviceRegistrationID);
		RequestManager.sendRequest(request);
	}

	static void unregisterWithServer() throws Exception
	{
		UnregisterDeviceRequest request = new UnregisterDeviceRequest();
		RequestManager.sendRequest(request);
	}
}
