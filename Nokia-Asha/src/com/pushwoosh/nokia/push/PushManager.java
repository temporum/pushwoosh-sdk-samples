//
//  PushManager.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed

package com.pushwoosh.nokia.push;

import java.util.Hashtable;
import java.util.Vector;

import com.nokia.notifications.NotificationEnvironment;
import com.nokia.notifications.NotificationError;
import com.nokia.notifications.NotificationException;
import com.nokia.notifications.NotificationInfo;
import com.nokia.notifications.NotificationMessage;
import com.nokia.notifications.NotificationSession;
import com.nokia.notifications.NotificationSessionFactory;
import com.nokia.notifications.NotificationSessionListener;
import com.nokia.notifications.NotificationState;
import com.pushwoosh.nokia.push.utils.PreferenceUtils;

import org.json.me.JSONException;
import org.json.me.JSONObject;
import javax.microedition.midlet.MIDlet;

/**
 * Push notifications manager
 */
public class PushManager implements NotificationSessionListener {
	
	/**
	 * Push notifications listener
	 */
	public interface PushNotificationsListener {
		/**
		 * Called when registration is successful
		 * 
		 * @param pushToken
		 *            received pushToken
		 */
		public void onRegistered(String pushToken);
		
		/**
		 * Called when push notification has been received
		 * 
		 * @param pushPayload
		 *            received pushPayload
		 */
		public void onPushNotificationReceived(String pushPayload);

		/**
		 * Called when unregistration is successful
		 * 
		 */
		public void onUnregistered();

		/**
		 * Called when request failed
		 * 
		 * @param e
		 *            Exception
		 */
		public void onErrorRegistering(Exception e);
		public void onErrorUnregistering(Exception e);
	}
	/**
	 * Set tags listener
	 */
	public interface SetTagsListener {
		/**
		 * Called when tags has been set
		 * 
		 * @param tags
		 *            skipped tags map
		 */
		public void onTagsSkipped(Vector tags);

		/**
		 * Called when request failed
		 * 
		 * @param e
		 *            Exception
		 */
		public void onError(Exception e);
	}
	/**
	 * Get tags listener
	 */
	public interface GetTagsListener {
		/**
		 * Called when tags received
		 * 
		 * @param tags
		 *            received tags map
		 */
		public void onTagsReceived(Hashtable tags);

		/**
		 * Called when request failed
		 * 
		 * @param e
		 *            Exception
		 */
		public void onError(Exception e);
	}
	
	NotificationSession session = null;
	String notificationId = null;
	String pushwooshAppId = null;
	
	PushNotificationsListener listener = null;

	/**
	 * Init push manager
	 * 
	 * @param pushwooshAppId
	 * 			Application Id in Pushwoosh Control Panel
	 * 
	 * @param serviceId
	 * 			Service ID from Nokia API Developer Console
	 * 
	 * @param application Id
	 * 			Application ID from Nokia API Developer Console
	 */
	public PushManager(MIDlet midlet, String pushwooshAppId, String serviceId, String applicationId, PushNotificationsListener listener) {
		this.pushwooshAppId = pushwooshAppId;
		this.listener = listener;
		PreferenceUtils.setApplicationId(pushwooshAppId);
	
		try {
			session = NotificationSessionFactory.openSession(midlet, serviceId, applicationId, this);
		} catch (Exception e) {
			listener.onErrorRegistering(e);
			return;
		}
	}
	
	/**
	 * Tracks application open. Call this function on start up of the app.
	 * 
	 */
	public void onStartup() {
		sendAppOpen();
	}

	/**
	 * Registers for Push Notifications. Note that you have to call this function on startup to receive push messages pending.
	 * 
	 */
	public void registerForPushNotifications() {
		try {
			//Have to always register application, even to get push messages pending. Simply opening session is not enough.
			//This will trigger whole registration mechanism as a result.
			session.registerApplication();
		} catch (Exception e) {
			listener.onErrorRegistering(e);
		}
	}

	/**
	 * Unregister from push notifications
	 */
	public void unregisterFromPushNotifications() {
		try {
			session.unregisterApplication();
			
			//unregister from Pushwoosh
			new Thread(new Runnable() {
				public void run()
				{
					try {
						DeviceRegistrar.unregisterWithServer();
						listener.onUnregistered();
					} catch (Exception e) {
						e.printStackTrace();
						listener.onErrorUnregistering(e);
						return;
					}
				}
			}).start();
			
		} catch (Exception e) {
			listener.onErrorUnregistering(e);
		}
	}

	/**
	 * Get push notification user data
	 * 
	 * @return string user data, or null
	 */
	 public String getCustomData(String payload) {
		try {
			JSONObject jsonPayload = new JSONObject(payload);
			return jsonPayload.getString("u");
		} catch (JSONException e) {
			e.printStackTrace();
			return null;
		}		 
	 }

	/**
	 * Send tags asynchronously
	 * 
	 * @param context
	 * @param tags
	 *            tags to send. Value can be String or Integer only - if not
	 *            Exception will be thrown
	 * @param callBack
	 *            execute result callback
	 */
	public static void sendTags(final Hashtable tags, final SetTagsListener callBack) {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					Vector skipped = DeviceFeature2_5.sendTags(tags);
					callBack.onTagsSkipped(skipped);
				} catch (Exception e) {
					e.printStackTrace();
					callBack.onError(e);
				}
			}
		}).start();
	}
	
	/**
	 * Get tags from Pushwoosh service synchronously. Do not call on the main thread as this will block the UI.
	 * 
	 * @param context
	 * @return tags, or null
	 */
	 public static Hashtable getTagsSync() {
		 try {
			return DeviceFeature2_5.getTags();
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	 }

	/**
	 * Get tags from Pushwoosh service asynchronously
	 * 
	 * @param context
	 * @return tags, or null
	 */
	public static void getTags(final GetTagsListener listener) {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					Hashtable tags = DeviceFeature2_5.getTags();
					listener.onTagsReceived(tags);
				} catch (Exception e) {
					e.printStackTrace();
					listener.onError(e);
					return;
				}
			}
		}).start();
	}

	/**
	 * Registers on Pushwoosh service asynchronously
	 * 
	 * @param context
	 * @param regId
	 *            registration ID
	 */
	private  void registerOnPushWoosh(final String regId) {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					DeviceRegistrar.registerWithServer(regId);
					listener.onRegistered(regId);
				} catch (Exception e) {
					e.printStackTrace();
					listener.onErrorRegistering(e);
					return;
				}
			}
		}).start();
	}

	/**
	 * Sends push stat asynchronously
	 * 
	 * @param context
	 * @param hash
	 */
	private static void sendPushStat(final String hash) {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					DeviceFeature2_5.sendPushStat(hash);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
	}

	/**
	 * Sends service message that app has been opened
	 * 
	 * @param context
	 */
	private static void sendAppOpen() {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					DeviceFeature2_5.sendAppOpen();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
	}
	
	/**
	 * Sends goal achieved asynchronously
	 * 
	 * @param context
	 * @param goal
	 * @param count
	 */
	public static void sendGoalAchieved(final String goal, final Integer count) {
		new Thread(new Runnable() {
			public void run()
			{
				try {
					DeviceFeature2_5.sendGoalAchieved(goal, count);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
	}

	static public Hashtable incrementalTag(Integer intValue) {
		Hashtable result = new Hashtable();
		result.put("operation", "increment");
		result.put("value", intValue);

		return result;
	}

	// NotificationSessionListener callbacks
	
    /**
     * @see com.nokia.notifications.NotificationSessionListener#infoReceived(NotificationInfo) 
     */
	public void infoReceived(NotificationInfo info) {
		System.out.println("INFO RECEIVED");

		String notificationId = info.getNotificationId();

		if (notificationId.length() == 0) {
			listener.onErrorRegistering(null);
		} else {
			this.notificationId = notificationId;
			registerOnPushWoosh(notificationId);
		}
	}

    /**
     * @see com.nokia.notifications.NotificationSessionListener#messageReceived(NotificationMessage) 
     */
	public void messageReceived(NotificationMessage message) {
		String payload = message.getPayload().getData();
		System.out.println("MESSAGE RECEIVED: " + payload);
		
		try {
			JSONObject jsonPayload = new JSONObject(payload);
			
			// send pushwoosh callback
			sendPushStat(jsonPayload.getString("p"));

		} catch (JSONException e) {
			e.printStackTrace();
		}
	
		listener.onPushNotificationReceived(payload);
	}

    /**
     * @see com.nokia.notifications.NotificationSessionListener#stateChanged(NotificationState) 
     */
	public void stateChanged(NotificationState state) {
		switch (state.getSessionState()) {
		case NotificationState.STATE_OFFLINE:
			break;
		case NotificationState.STATE_CONNECTING:
			break;
		case NotificationState.STATE_ONLINE:
			int env = -1;
			try {
				env = session.getEnvironment();
			} catch (NotificationException ne) {
				System.out.println(ne.toString());
			}

			switch (env) {
			case NotificationEnvironment.SANDBOX:
				System.out.println("Online (Sandbox)");
				break;
			case NotificationEnvironment.GLOBAL_PRODUCTION:
				System.out.println("Online (Production)");
				break;
			case NotificationEnvironment.CHINA_PRODUCTION:
				System.out.println("Online (China Production)");
				break;
			case NotificationEnvironment.OTHER_ENVIRONMENT:
				System.out.println("Online (Others)");
				break;
			}
			
			//get the notification ID callback
			retrieveNotificationId();
			break;
		default:
			System.out.println("Unknown notification state");
			break;
		}

		final int error = state.getSessionError();

		if (error != NotificationError.ERROR_NONE) {
			System.out.println(notificationStateErrorCodeToString(error));
		}
	}
	
	void retrieveNotificationId() {
		try {
			session.getNotificationInformation();
		} catch (NotificationException e) {
			listener.onErrorRegistering(e);
		}
	}

	/**
	 * Provides an error message matching the given error code.
	 * 
	 * @param errorCode
	 *            The error code from NotificationState::getSessionError().
	 * @return A newly created string containing the error message.
	 */
	private String notificationStateErrorCodeToString(final int errorCode) {
		String errorMessage = null;

		switch (errorCode) {
		case NotificationError.ERROR_APPLICATION_ID_CONFLICT:
			errorMessage = "The Application ID has already been registered by another MIDlet.";
			break;
		case NotificationError.ERROR_APPLICATION_ID_INVALID:
			errorMessage = "The Application ID is either empty or more than 255 characters long.";
			break;
		case NotificationError.ERROR_AUTHENTICATION_FAILED:
			errorMessage = "The device is not able to fetch the authentication details from the Notification server.";
			break;
		case NotificationError.ERROR_CONNECTION_DISABLED_BY_USER:
			errorMessage = "The user has disabled connections like WiFi and cellular network.";
			break;
		case NotificationError.ERROR_DISABLED_BY_USER:
			errorMessage = "The user has disabled  notifications.";
			break;
		case NotificationError.ERROR_NO_NETWORK:
			errorMessage = "The Notification Enabler has lost connection with the Notification server.";
			break;
		case NotificationError.ERROR_NONE:
			errorMessage = "No error.";
			break;
		case NotificationError.ERROR_NOT_ALLOWED:
			errorMessage = "NotificationSession is not in correct state.";
			break;
		case NotificationError.ERROR_NOT_KNOWN:
			errorMessage = "Other Notification error.";
			break;
		case NotificationError.ERROR_NOT_REGISTERED:
			errorMessage = "MIDlet is not registered, but is calling for a function that requires registration.";
			break;
		case NotificationError.ERROR_REGISTER_FAILED:
			errorMessage = "Registration failed because there is not enough space available in the device memory to store the settings.";
			break;
		case NotificationError.ERROR_SERVICE_UNAVAILABLE:
			errorMessage = "Notification Server is not available.";
			break;
		case NotificationError.ERROR_SESSION_CLOSED:
			errorMessage = "NotificationSession function call failed because session was closed.";
			break;
		default:
			errorMessage = "Unknown error.";
			break;
		}

		return errorMessage;
	}
}
