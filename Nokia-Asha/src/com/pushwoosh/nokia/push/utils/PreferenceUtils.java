//
//  MessageActivity.java
//
// Pushwoosh Push Notifications SDK
// www.pushwoosh.com
//
// MIT Licensed

package com.pushwoosh.nokia.push.utils;

import javax.microedition.rms.RecordEnumeration;
import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;

import java.util.Hashtable;

public class PreferenceUtils
{
	private static PreferenceUtils instance = null;
	
	public static PreferenceUtils instance() {
		if(instance == null)
			instance = new PreferenceUtils();
		
		return instance;
	}
	
	private PreferenceUtils() {
		try {
			openRecordStore();
			getPushSettings();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (RecordStoreException e) {
			e.printStackTrace();
		}
	}
	
	// Name of record store of application.
    private final String RECORD_STORE_NAME = "PushSettingsStore";
    
    // Record store of application.
    private RecordStore recordStore;
    
    // Array of settings deserialized from records in record store.
    private Hashtable pushSettings = new Hashtable();
    	
    /**
     * Opens named record store of application. Record store will be created 
     * if it is not exists.
     */
    private void openRecordStore() throws RecordStoreException, SecurityException {
        recordStore = RecordStore.openRecordStore(RECORD_STORE_NAME, true);
    }
    
    /**
     * Closes record store.
     */
    private void closeRecordStore(){
        try {
            recordStore.closeRecordStore();
        } catch (RecordStoreException ex) {
            // Do nothing.
        }
    }
    
    /**
     * Deletes record store and creates it again.
     */
    private void deleteAllSettings() {
        try {
            recordStore.closeRecordStore();
            RecordStore.deleteRecordStore(RECORD_STORE_NAME);
 
            openRecordStore();
 
        } catch(RecordStoreException rsExc) {
        	rsExc.printStackTrace();
        }   
    }
    
    /**
     * Reads settings from record store and adds it to array of settings.
     * @throws RecordStoreException if error occurs on working with record store.
     */
    private void getPushSettings() throws RecordStoreException {
        pushSettings.clear();
 
        RecordEnumeration recEnum = recordStore.enumerateRecords(null, 
                null, false);
        while(recEnum.hasNextElement() == true) {
            int settingId = recEnum.nextRecordId();
            byte[] data = recordStore.getRecord(settingId);
            PushSetting setting = PushSetting.deserialize(data);
            if(setting != null) {
                pushSettings.put(setting.getName(), setting.getValue());
            }          
        }
        
        recEnum.destroy();
    }
    
    /**
     * Adds setting to record store.
     * @param name - name of new record.
     * @param value - value of new record.
     */
    private void addSetting(String name, String value) {
        try {
            PushSetting setting = new PushSetting(name, value);
            pushSettings.put(setting.getName(), setting.getValue());
            byte[] data = setting.serialize();
            recordStore.addRecord(data, 0, data.length);
        } catch(RecordStoreException rsExc) {
        	rsExc.printStackTrace();
        }
    }
    
    private String getSetting(String name) {
    	return (String)pushSettings.get(name);
    }

	public static String getSenderId()
	{
		return instance().getSetting("SENDER");
	}

	public static void setSenderId(String senderId)
	{
		instance().addSetting("SENDER", senderId);
	}

	public static long getLastRegistration()
	{
		return Long.parseLong(instance().getSetting("LAST_REGISTRATION"));
	}

	public static void setLastRegistration(long lastRegistrationTime)
	{
		instance().addSetting("LAST_REGISTRATION", String.valueOf(lastRegistrationTime));
	}

	public static void setApplicationId(String applicationId)
	{
		instance().addSetting("PW_APP_ID", applicationId);
	}

	public static String getApplicationId()
	{
		return instance().getSetting("PW_APP_ID");
	}
}
