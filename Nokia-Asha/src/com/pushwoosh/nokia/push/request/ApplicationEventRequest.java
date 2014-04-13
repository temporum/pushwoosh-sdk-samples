package com.pushwoosh.nokia.push.request;

import java.util.Hashtable;

import org.json.me.JSONException;

public class ApplicationEventRequest extends PushRequest {
	
	private String goal;
	private Integer count;
	
	public ApplicationEventRequest(String goal, Integer count) {
		this.goal = goal;
		this.count = count;
	}

	public String methodName() {
		return "applicationEvent";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable hash = super.requestDictionary();
		
		hash.put("goal", goal);
		
		if(count != null)
			hash.put("count", count);
		
		return hash;
	}
}
