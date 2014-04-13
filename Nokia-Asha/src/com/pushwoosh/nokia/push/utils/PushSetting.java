package com.pushwoosh.nokia.push.utils;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
 
/**
 * Setting of application. Consist of pair "name - value"
 */
public class PushSetting {
    // Name of setting.
    private String name;
    // Value of setting.
    private String value;
 
    /**
     * Constructor.
     * @param name - name of new setting.
     * @param value - value of new setting.
     */
    public PushSetting(String name, String value) {
        this.name = name;
        this.value = value;
    }
 
    /**
     * Deserializes app setting from data buffer and returns it.
     * @param data - data for creating new setting.
     * @return new setting.
     */
    public static PushSetting deserialize(byte[] data) {
        String name = null;
        String value = null;
 
        try {
            ByteArrayInputStream byteInStream = new ByteArrayInputStream(data);
            DataInputStream dataInStream = new DataInputStream(byteInStream);
 
            name = dataInStream.readUTF();
            value = dataInStream.readUTF();
 
            dataInStream.close();
            byteInStream.close();
 
        } catch(IOException exc) {
            return null;
        }
 
        return new PushSetting(name, value);
    }
 
    /**
     * Serilizes setting to array of bytes.
     * @return array of bytes representing serialized setting.
     */
    public byte[] serialize() {
        byte[] data = null;
 
        try {
            ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
            DataOutputStream dataOutStream = new DataOutputStream(byteOutStream);
 
            dataOutStream.writeUTF(name);
            dataOutStream.writeUTF(value);
 
            dataOutStream.flush();
            data = byteOutStream.toByteArray();
 
            dataOutStream.close();
            byteOutStream.close();
 
        } catch(IOException exc) {
            return null;
        }
 
        return data;
    }
 
    /**
     * Gets name of setting.
     * @return name of setting.
     */
    public String getName() {
        return name;
    }
 
    /**
     * Gets value of setting.
     * @return value of setting.
     */
    public String getValue() {
        return value;
    }
 
    /**
     * Returns textual representation of setting.
     * @return textual representation of setting.
     */
    public String toString() {
        return name + ": " + value;
    }
}
