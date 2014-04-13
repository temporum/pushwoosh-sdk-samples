package com.pushwoosh.nokia.push.data;

public class PushZoneLocation
{
	private String mName;
	private double mLat;
	private double mLng;
	private long mDistanceTo;

	public long getDistanceTo()
	{
		return mDistanceTo;
	}

	public void setName(String name)
	{
		mName = name;
	}

	public void setLat(double lat)
	{
		mLat = lat;
	}

	public void setLng(double lng)
	{
		mLng = lng;
	}

	public void setDistanceTo(long distanceTo)
	{
		mDistanceTo = distanceTo;
	}
}
