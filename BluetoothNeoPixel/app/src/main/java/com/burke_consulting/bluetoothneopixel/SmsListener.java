package com.burke_consulting.bluetoothneopixel;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.telephony.SmsMessage;
import android.util.Log;

/**
 * Created by darrylb on 26/05/16.
 */
public class SmsListener extends BroadcastReceiver {

    private SharedPreferences preferences;

    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO Auto-generated method stub

        if(intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED")){
            Bundle bundle = intent.getExtras();           //---get the SMS message passed in---
            SmsMessage[] msgs = null;
            String msg_from;
            if (bundle != null){
                //---retrieve the SMS message received---
                try{
                    Object[] pdus = (Object[]) bundle.get("pdus");
                    msgs = new SmsMessage[pdus.length];
                    for(int i=0; i<msgs.length; i++){
                        msgs[i] = SmsMessage.createFromPdu((byte[])pdus[i]);
                        msg_from = msgs[i].getOriginatingAddress();
                        String msgBody = msgs[i].getMessageBody();
                        String msgSender = msgs[i].getOriginatingAddress();
                        Log.d("BT", "SMS:["+msgSender+"][" + msgBody+"]");
                        if (msgBody.equals("?")){

                            SmsManager smsManager = SmsManager.getDefault();
                            smsManager.sendTextMessage(msgSender, null, "Command start with '..' Commands: A00000 [all off], COP, COPCAR, WORM, CYCLONE, STROBE,RAINBOW,RAINBOWS,FIRE ", null, null);

                        }

                        if (msgBody.startsWith("..")){
                            msgBody=msgBody.substring(2);

                            //msgBody.replaceFirst("\\.\\.","");
                            main.SendToBT(msgBody+"|");
                        } else {
                            Log.d("BT", "SMS: Ignoring SMS");
                        }
                    }
                }catch(Exception e){
//                            Log.d("Exception caught",e.getMessage());
                }
            }
        }
    }
    MainActivity main = null;
    void setMainActivityHandler(MainActivity main){
        this.main=main;
    }
}
