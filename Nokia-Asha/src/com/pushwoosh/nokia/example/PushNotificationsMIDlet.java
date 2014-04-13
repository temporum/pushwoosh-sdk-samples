package com.pushwoosh.nokia.example;

import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

public class PushNotificationsMIDlet extends MIDlet {

	public PushNotificationsMIDlet() {
		// TODO Auto-generated constructor stub
	}

	protected void destroyApp(boolean arg0) throws MIDletStateChangeException {
		// TODO Auto-generated method stub

	}

	protected void pauseApp() {
		// TODO Auto-generated method stub

	}

	protected void startApp() throws MIDletStateChangeException
	{
		Display.getDisplay(this).setCurrent(new PushwooshForm(this));
	}

}
