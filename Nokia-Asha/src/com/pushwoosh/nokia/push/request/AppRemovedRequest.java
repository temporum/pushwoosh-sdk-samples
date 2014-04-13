package com.pushwoosh.nokia.push.request;

import java.util.Hashtable;

import org.json.me.JSONException;

public class AppRemovedRequest extends PushRequest {
	
	private String packageName;
	
	public AppRemovedRequest(String packageName) {
		this.packageName = packageName;
	}

	public String methodName() {
		return "androidPackageRemoved";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable data = super.requestDictionary();	
		data.put("android_package", packageName);
		
		return data;
	}
}
