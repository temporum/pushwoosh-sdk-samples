package com.pushwoosh.nokia.push.utils;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import org.json.me.JSONArray;
import org.json.me.JSONException;
import org.json.me.JSONObject;

/**
 * Date: 07.04.2014
 * Time: 18:38
 *
 * @author Yuri Shmakov
 */
public class JsonUtils
{
	public static JSONObject mapToJson(Hashtable tags)
	{
		JSONObject object = new JSONObject();

		Enumeration keys = tags.keys();
		for (; keys.hasMoreElements(); )
		{
	        /*
             * Deviate from the original by checking that keys are non-null and
             * of the proper type. (We still defer validating the values).
             */
			String key = (String) keys.nextElement();
			if (key == null)
			{
				throw new NullPointerException("key == null");
			}
			try
			{
				object.put(key, wrap(tags.get(key)));
			}
			catch (JSONException e)
			{
				e.printStackTrace();
			}
		}

		return object;
	}

	public static JSONArray collectionToJson(Vector data)
	{
		JSONArray jsonArray = new JSONArray();
		if (data != null)
		{
			for (int i = 0; i < data.size(); i++)
			{
				jsonArray.put(wrap(data.elementAt(i)));
			}
		}
		return jsonArray;
	}

	public static JSONArray arrayToJson(Object data) throws JSONException
	{
		if (!data.getClass().isArray())
		{
			throw new JSONException("Not a primitive data: " + data.getClass());
		}
		final int length = Array.getLength(data);
		JSONArray jsonArray = new JSONArray();
		for (int i = 0; i < length; ++i)
		{
			jsonArray.put(wrap(Array.get(data, i)));
		}

		return jsonArray;
	}

	private static Object wrap(Object o)
	{
		if (o == null)
		{
			return null;
		}
		if (o instanceof JSONArray || o instanceof JSONObject)
		{
			return o;
		}
		try
		{
			if (o instanceof Vector)
			{
				return collectionToJson((Vector) o);
			}
			else if (o.getClass().isArray())
			{
				return arrayToJson(o);
			}
			if (o instanceof Hashtable)
			{
				return mapToJson((Hashtable) o);
			}
			if (o instanceof Boolean ||
					o instanceof Byte ||
					o instanceof Character ||
					o instanceof Double ||
					o instanceof Float ||
					o instanceof Integer ||
					o instanceof Long ||
					o instanceof Short ||
					o instanceof String)
			{
				return o;
			}
		}
		catch (Exception ignored)
		{
		}
		return null;
	}
}
