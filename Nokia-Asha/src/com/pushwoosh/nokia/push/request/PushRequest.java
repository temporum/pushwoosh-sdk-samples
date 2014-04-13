package com.pushwoosh.nokia.push.request;

import java.util.Hashtable;

import org.json.me.JSONException;
import org.json.me.JSONObject;

import com.pushwoosh.nokia.push.utils.GeneralUtils;
import com.pushwoosh.nokia.push.utils.PreferenceUtils;

public abstract class PushRequest {
	
	String response;
	
	public abstract String methodName();
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable hash = new Hashtable();
		
		hash.put("application", PreferenceUtils.getApplicationId());
		hash.put("hwid", GeneralUtils.getDeviceUUID());
		hash.put("v", "1.0");	//SDK version
			
		return hash;
	}
	
	public void parseResponse(JSONObject response) throws JSONException {
		this.response = response.toString();
	}
	
	public String getRawResponse() {
		return response;
	}
}
