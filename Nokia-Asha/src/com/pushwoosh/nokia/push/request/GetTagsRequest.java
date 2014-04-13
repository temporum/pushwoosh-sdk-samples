package com.pushwoosh.nokia.push.request;

import java.util.Enumeration;
import java.util.Hashtable;

import org.json.me.JSONException;
import org.json.me.JSONObject;

public class GetTagsRequest extends PushRequest {
	
	private Hashtable tags;

	public String methodName() {
		return "getTags";
	}
	
	public void parseResponse(JSONObject resultData) throws JSONException {
		Hashtable result = new Hashtable();

		JSONObject response = resultData.getJSONObject("response");
		JSONObject jsonResult = response.getJSONObject("result");
		
		Enumeration keys = jsonResult.keys();
		while(keys.hasMoreElements()) {
			String key = (String)keys.nextElement();
			result.put(key, jsonResult.get(key));
		}
		
		tags = result;
	}
	
	public Hashtable getTags() {
		return tags;
	}
}
