package com.pushwoosh.nokia.example;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Displayable;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;
import com.pushwoosh.nokia.push.PushManager;
import com.pushwoosh.nokia.push.PushManager.PushNotificationsListener;

public class PushwooshForm extends Form implements PushNotificationsListener, CommandListener
{
	PushManager mPushManager;
	
	Command register;
	Command unregister;
	
	String notificationId = null;
	
	public PushwooshForm(MIDlet midlet)
	{
		super("Push Notifications");

		mPushManager = new PushManager(midlet, "A0443-C41F6", "com.pushwoosh.nokia.test", "test.nokia.pushwoosh.com", this);
		
		register = new Command("Register for pushes", Command.OK, 1);
		unregister = new Command("Unregister from pushes", Command.OK, 1);
		
		addCommand(register);
		
		setCommandListener(this);
	}
	
	public void onRegistered(String pushToken) {
		removeCommand(register);
		addCommand(unregister);
		
		System.out.println("Push token: " + pushToken);
	}
	public void onPushNotificationReceived(String pushPayload) {
		System.out.println("Push payload: " + pushPayload);
	}
	public void onUnregistered() {
		removeCommand(unregister);
		addCommand(register);
		
		System.out.println("Unregistered!");
	}
	public void onErrorRegistering(Exception e) {
		System.out.println("Registering error: " + e.getMessage());
	}
	public void onErrorUnregistering(Exception e) {
		System.out.println("Unregistering error: " + e.getMessage());
	}

	public void commandAction(Command c, Displayable d) {
		if(c == register)
		{
			mPushManager.registerForPushNotifications();
		}
		else if(c == unregister)
		{
			mPushManager.unregisterFromPushNotifications();
		}
	}
}
