package com.pushwoosh.nokia.push.request;

import java.util.Calendar;
import java.util.Hashtable;

import org.json.me.JSONException;

public class RegisterDeviceRequest extends PushRequest {
	
	private String pushToken;
	
	public RegisterDeviceRequest(String pushToken) {
		this.pushToken = pushToken;
	}

	public String methodName() {
		return "registerDevice";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable data = super.requestDictionary();
		
//		data.put("device_name", GeneralUtils.isTablet(context) ? "Tablet" : "Phone");
		
		//TODO: not android
		data.put("device_type", "3");

		data.put("v", "1.0");
		data.put("language", System.getProperty("microedition.locale"));
		data.put("timezone", new Integer(Calendar.getInstance().getTimeZone().getRawOffset() / 1000)); // converting from milliseconds to seconds

		data.put("push_token", pushToken);

		return data;
	}
}
