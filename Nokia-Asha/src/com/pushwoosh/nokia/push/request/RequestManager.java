//
//RequestManager.java
//
//Pushwoosh Push Notifications SDK
//www.pushwoosh.com
//
//MIT Licensed
package com.pushwoosh.nokia.push.request;

import org.json.me.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;


public class RequestManager
{
	public static final int MAX_TRIES = 1;
	public static final String PUSH_VERSION = "1.3";
	
	public static boolean useSSL = false;
	
	private static final String BASE_URL_SECURE = "https://cp.pushwoosh.com/json/" + PUSH_VERSION + "/";
	private static final String BASE_URL = "http://cp.pushwoosh.com/json/" + PUSH_VERSION + "/";
	
	public static void sendRequest(PushRequest request) throws Exception
	{
		Hashtable data = request.requestDictionary();

		System.out.println("Try To sent: " + request.methodName());

		NetworkResult res = new NetworkResult(500, 0, null);
		Exception exception = new Exception();
		
		for (int i = 0; i < MAX_TRIES; ++i)
		{
			try
			{
				res = makeRequest(data, request.methodName());
				if (200 != res.getResultCode())
				{
					continue;
				}

				if (200 != res.getPushwooshCode())
				{
					break;
				}
				
				System.out.println(request.methodName() + " response success");
				
				JSONObject response = res.getResultData();
				if (response != null)
				{
					request.parseResponse(response);
				}

				return;
			}
			catch (Exception ex)
			{
				exception = ex;
			}
		}

		System.out.println("ERROR: " + exception.getMessage() + ". Response = " + res.getResultData());
		throw exception;
	}
	
	private static NetworkResult makeRequest(Hashtable data, String methodName) throws Exception
	{
		NetworkResult result = new NetworkResult(500, 0, null);
		OutputStream connectionOutput = null;
		InputStream inputStream = null;
		try
		{
			String urlString = BASE_URL + methodName;
			if(useSSL)
				urlString = BASE_URL_SECURE + methodName;
			
			HttpConnection connection = (HttpConnection) Connector.open(urlString);
		    if (connection == null) {
		        throw new IOException("No network access");
		    }

			connection.setRequestMethod("POST");
			connection.setRequestProperty("Content-Type", "application/json; charset=utf-8");
	
			JSONObject innerRequestJson = new JSONObject();
	
			for (Enumeration keys = data.keys(); keys.hasMoreElements();)
			{
				String key = (String) keys.nextElement();
				innerRequestJson.put(key, data.get(key));
			}
	
			JSONObject requestJson = new JSONObject();
			requestJson.put("request", innerRequestJson);
	
			connection.setRequestProperty("Content-Length", String.valueOf(requestJson.toString().getBytes().length));
	
			connectionOutput = connection.openDataOutputStream();
			connectionOutput.write(requestJson.toString().getBytes());
			connectionOutput.flush();
			connectionOutput.close();
	
			inputStream = connection.openDataInputStream();
	
			ByteArrayOutputStream dataCache = new ByteArrayOutputStream();
	
			// Fully read data
			byte[] buff = new byte[1024];
			int len;
			while ((len = inputStream.read(buff)) >= 0)
			{
				dataCache.write(buff, 0, len);
			}
	
			// Close streams
			dataCache.close();
	
			String jsonString = new String(dataCache.toByteArray()).trim();
			System.out.println("PushWooshResult: " + jsonString);
	
			JSONObject resultJSON = new JSONObject(jsonString);
	
			result.setData(resultJSON);
			result.setCode(connection.getResponseCode());
			result.setPushwooshCode(resultJSON.getInt("status_code"));
		}
		finally
		{
			if (null != inputStream)
			{
				inputStream.close();
			}
			if (null != connectionOutput)
			{
				connectionOutput.close();
			}
		}
	
		return result;
	}
	
	public static class NetworkResult
	{
		private int mPushwooshCode;
		private int mResultCode;
		private JSONObject mResultData;
	
		public NetworkResult(int networkCode, int pushwooshCode, JSONObject data)
		{
			mResultCode = networkCode;
			mPushwooshCode = pushwooshCode;
			mResultData = data;
		}
	
		public void setCode(int code)
		{
			mResultCode = code;
		}
	
		public void setPushwooshCode(int code)
		{
			mPushwooshCode = code;
		}
	
		public void setData(JSONObject data)
		{
			mResultData = data;
		}
	
		public int getResultCode()
		{
			return mResultCode;
		}
	
		public int getPushwooshCode()
		{
			return mPushwooshCode;
		}
	
		public JSONObject getResultData()
		{
			return mResultData;
		}
	}
}
