package org.greedysky.ttkmobile;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import android.widget.Toast;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

public class TTKMobile extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static TTKMobile m_instance;

    public TTKMobile()
    {
        m_instance = this;
    }

    public static void showMessageBox(String str)
     {
         final String text = str;
         m_instance.runOnUiThread(new Runnable(){
             @Override
             public void run(){
                 Toast.makeText(m_instance, text, Toast.LENGTH_SHORT).show();
             }
         });
    }

    public static int currentNetIsWifi()
    {
        ConnectivityManager con = (ConnectivityManager)m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mobileNetworkInfo = con.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
        NetworkInfo wifiNetworkInfo = con.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

        if(wifiNetworkInfo != null && mobileNetworkInfo != null){
            NetworkInfo.State gprs = mobileNetworkInfo.getState();
            NetworkInfo.State wifi = wifiNetworkInfo.getState();
            if(gprs == NetworkInfo.State.CONNECTED || gprs == NetworkInfo.State.CONNECTING){
                return 0;
            }
            if(wifi == NetworkInfo.State.CONNECTED || wifi == NetworkInfo.State.CONNECTING){
                return 1;
            }
        }
        return -1;
    }
}
