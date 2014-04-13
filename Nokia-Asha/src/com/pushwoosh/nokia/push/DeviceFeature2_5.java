//
// DeviceFeature2_5.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed
package com.pushwoosh.nokia.push;

import java.util.Hashtable;
import java.util.Vector;

import com.pushwoosh.nokia.push.request.*;

public class DeviceFeature2_5
{
	public static void sendPushStat(String hash) throws Exception
	{
		PushStatRequest request = new PushStatRequest(hash);
		RequestManager.sendRequest(request);
	}

	public static void sendGoalAchieved(String goal, Integer count) throws Exception
	{
		ApplicationEventRequest request = new ApplicationEventRequest(goal, count);
		RequestManager.sendRequest(request);
	}

	public static void sendAppOpen() throws Exception
	{
		AppOpenRequest request = new AppOpenRequest();
		RequestManager.sendRequest(request);
	}

	public static Vector sendTags(Hashtable tags) throws Exception
	{
		SetTagsRequest request = new SetTagsRequest(tags);
		RequestManager.sendRequest(request);
		
		return request.getSkippedTags();
	}

	public static void sendMessageDeliveryEvent(String hash) throws Exception
	{
		MessageDeliveredRequest request = new MessageDeliveredRequest(hash);
		RequestManager.sendRequest(request);
	}

	static void sendAppRemovedData(String packageName) throws Exception
	{
		AppRemovedRequest request = new AppRemovedRequest(packageName);
		RequestManager.sendRequest(request);
	}

	public static Hashtable getTags() throws Exception
	{
		GetTagsRequest request = new GetTagsRequest();
		RequestManager.sendRequest(request);
		return request.getTags();
	}
}
