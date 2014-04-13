package com.pushwoosh.nokia.push.request;

import java.util.Hashtable;

import org.json.me.JSONException;

public class PushStatRequest extends PushRequest {
	private String hash;
	
	public PushStatRequest(String hash) {
		this.hash = hash;
	}

	public String methodName() {
		return "pushStat";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable hashtable = super.requestDictionary();
		
		if(hash != null)
			hashtable.put("hash", hash);
		
		return hashtable;
	}
}
