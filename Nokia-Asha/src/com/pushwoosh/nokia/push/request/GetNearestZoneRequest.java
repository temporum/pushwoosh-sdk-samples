package com.pushwoosh.nokia.push.request;

import java.util.Hashtable;

import org.json.me.JSONException;
import org.json.me.JSONObject;

import com.pushwoosh.nokia.push.data.PushZoneLocation;

public class GetNearestZoneRequest extends PushRequest {
	
//	private Location location;
	private PushZoneLocation zoneLocation;
	
	public GetNearestZoneRequest() {
//		this.location = location;
	}

	public String methodName() {
		return "getNearestZone";
	}
	
	public Hashtable requestDictionary() throws JSONException {
		Hashtable data = super.requestDictionary();
		
//		data.put("lat", location.getLatitude());
//		data.put("lng", location.getLongitude());
		
		return data;
	}
	
	public void parseResponse(JSONObject response) throws JSONException {
		JSONObject jsonResp = response.getJSONObject("response");

		zoneLocation = new PushZoneLocation();

		zoneLocation.setName(jsonResp.getString("name"));
		zoneLocation.setLat(Double.parseDouble(jsonResp.get("lat").toString()));
		zoneLocation.setLng(Double.parseDouble(jsonResp.get("lng").toString()));
		zoneLocation.setDistanceTo(jsonResp.getLong("distance"));
	}	
	
	public PushZoneLocation getNearestLocation() {
		return zoneLocation;
	}
}
