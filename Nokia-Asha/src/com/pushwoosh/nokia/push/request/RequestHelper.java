//
//  RequestHelper.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed
package com.pushwoosh.nokia.push.request;

import com.pushwoosh.nokia.push.data.PushZoneLocation;
import com.pushwoosh.nokia.push.utils.GeneralUtils;
import com.pushwoosh.nokia.push.utils.PreferenceUtils;

import org.json.me.JSONException;
import org.json.me.JSONObject;

import java.util.Calendar;
import java.util.Enumeration;
import java.util.Hashtable;

public class RequestHelper
{
	static public Hashtable getRegistrationUnregistrationData(String deviceRegistrationID)
	{
		Hashtable data = new Hashtable();

		data.put("application", PreferenceUtils.getApplicationId());
		data.put("hwid", GeneralUtils.getDeviceUUID());
//		data.put("device_name", System.getProperty("microedition.platform"));
		
		//TODO:Nokia, not android
		data.put("device_type", "3");

		data.put("v", "1.0");
		data.put("language", System.getProperty("microedition.locale"));
		data.put("timezone", new Integer(Calendar.getInstance().getTimeZone().getRawOffset() / 1000)); // converting from milliseconds to seconds

		data.put("push_token", deviceRegistrationID);

		return data;
	}

	static public Hashtable getSendTagsData()
	{
		Hashtable data = new Hashtable();

		data.put("application", PreferenceUtils.getApplicationId());
		data.put("hwid", GeneralUtils.getDeviceUUID());

		return data;
	}

	static public PushZoneLocation getPushZoneLocationFromData(JSONObject resultData) throws JSONException
	{
		JSONObject response = resultData.getJSONObject("response");

		PushZoneLocation location = new PushZoneLocation();

		location.setName(response.getString("name"));
		location.setLat(Double.parseDouble((String)response.get("lat")));
		location.setLng(Double.parseDouble((String)response.get("lng")));
		location.setDistanceTo(response.getLong("distance"));

		return location;
	}
	
	static public Hashtable getAppRemovedData(String packageName) {
		Hashtable data = new Hashtable();

		data.put("application", PreferenceUtils.getApplicationId());
		data.put("android_package", packageName);
		data.put("hwid", GeneralUtils.getDeviceUUID());

		return data;
	}
	
	static public Hashtable getGetTagsData() {
		Hashtable data = new Hashtable();

		data.put("application", PreferenceUtils.getApplicationId());
		data.put("hwid", GeneralUtils.getDeviceUUID());

		return data;		
	}

	public static Hashtable getTagsFromData(JSONObject resultData) {
		Hashtable result = new Hashtable();

		try {
			JSONObject response = resultData.getJSONObject("response");
			JSONObject jsonResult = response.getJSONObject("result");
			
			Enumeration keys = jsonResult.keys();
			while(keys.hasMoreElements()) {
				String key = (String)keys.nextElement();
				result.put(key, jsonResult.get(key));
			}
		} catch (JSONException e) {
			e.printStackTrace();
			return new Hashtable();
		}

		return result;
	}
}
