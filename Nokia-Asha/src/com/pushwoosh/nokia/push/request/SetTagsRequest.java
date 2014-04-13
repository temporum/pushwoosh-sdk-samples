package com.pushwoosh.nokia.push.request;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import org.json.me.JSONArray;
import org.json.me.JSONException;
import org.json.me.JSONObject;

import com.pushwoosh.nokia.push.PushManager;
import com.pushwoosh.nokia.push.utils.JsonUtils;

public class SetTagsRequest extends PushRequest {
	
	private Hashtable tags;
	private Vector skippedTags;
	
	public SetTagsRequest(Hashtable tags) {
		this.tags = tags;
	}

	public String methodName() {
		return "setTags";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable data = super.requestDictionary();
		
		JSONObject tagsObject = jsonObjectFromTagMap(tags);
		data.put("tags", tagsObject);

		return data;
	}
	
	private JSONObject jsonObjectFromTagMap(Hashtable tags) throws JSONException
	{
		Enumeration keys = tags.keys();
		// prepare strange #pwinc# key
		for (; keys.hasMoreElements(); )
		{
			String key = (String) keys.nextElement();
			Object value = tags.get(key);
			if (value instanceof String)
			{
				String valString = (String) value;
				if (valString.startsWith("#pwinc#"))
				{
					valString = valString.substring(7);
					Integer intValue = new Integer(Integer.parseInt(valString));
					tags.put(key, jsonObjectFromTagMap(PushManager.incrementalTag(intValue)));
				}
			}
		}

		return JsonUtils.mapToJson(tags);
	}
	
	public void parseResponse(JSONObject response) throws JSONException {
		JSONObject resp = response.getJSONObject("response");
		JSONArray skippedTagsJson = resp.getJSONArray("skipped");
		
		skippedTags = new Vector();
		for(int i = 0; i < skippedTagsJson.length(); ++i) {
			String tag = (String)skippedTagsJson.get(i);
			skippedTags.addElement(tag);
		}
	}

	public Vector getSkippedTags() {
		return skippedTags;
	}
}
