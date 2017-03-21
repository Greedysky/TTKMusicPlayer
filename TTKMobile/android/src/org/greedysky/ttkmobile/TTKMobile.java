package org.greedysky.ttkmobile;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.AlertDialog;
import android.app.ProgressDialog;

import android.media.AudioManager;

import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;

import android.widget.Toast;

import android.net.Uri;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

public class TTKMobile extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static TTKMobile m_instance = null;
    ////////////////////////////////////////////////////////////////////
    private static NotificationManager m_notificationManager = null;
    private static Notification.Builder m_builder = null;
    ////////////////////////////////////////////////////////////////////
    private ProgressDialog progressDialog = null;
    ////////////////////////////////////////////////////////////////////
    private final String UPDATE_SERVERAPK = "ttkmobile.apk";
    private final String APK_URL = "http://7xpa0g.com1.z0.glb.clouddn.com/v";
    private final String APK = ".apk";
    private static String VERSION;
    ////////////////////////////////////////////////////////////////////

    public TTKMobile()
    {
        m_instance = this;
    }

    public static void notify(String title, String text, int value)
    {
        if (m_notificationManager == null)
        {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            m_builder = new Notification.Builder(m_instance);
            m_builder.setSmallIcon(R.drawable.icon);
            m_builder.setOngoing(true);
        }

        m_builder.setContentTitle( title );
        m_builder.setContentText( text );

        if( value < 0)
        {
            m_builder.setProgress(100, 100, false);
            m_notificationManager.notify(1, m_builder.build());
            return;
        }

        if( value < 100)
        {
            m_builder.setProgress(100, value, false);
            m_notificationManager.notify(1, m_builder.build());
        }
        else
        {
            m_notificationManager.cancelAll();
        }
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

    ////////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////////
    public static int getCurrentVolume()
    {
        AudioManager manager = (AudioManager)m_instance.getSystemService(Context.AUDIO_SERVICE);
        return manager.getStreamVolume(AudioManager.STREAM_MUSIC);
    }

    public static void setCurrentVolume(int volume)
    {
        AudioManager manager = (AudioManager)m_instance.getSystemService(Context.AUDIO_SERVICE);
        manager.setStreamVolume(AudioManager.STREAM_MUSIC, volume, 0);
    }

    ////////////////////////////////////////////////////////
    public static void updateApplicationDialog(String str)
    {
        VERSION = str;
        m_instance.runOnUiThread(new Runnable(){
            @Override
            public void run(){
                m_instance.updateDialog();
            }
        });
    }

    public void updateDialog(){
        new AlertDialog.Builder(m_instance)
            .setMessage("确定下载最新版本吗？")
            .setTitle("应用更新")
            .setPositiveButton("好的", new OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    progressDialog = new ProgressDialog(m_instance);
                    progressDialog.setTitle("正在下载");
                    progressDialog.setMessage("请稍后...");
                    progressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
                    progressDialog.setCancelable(false);
                    downFile(APK_URL + VERSION + APK);
                    dialog.dismiss();
                }
            })
            .setNegativeButton("下次更新", new OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    dialog.dismiss();
                }
            })
            .setCancelable(false)
            .show();
    }

    public void downFile(final String url){
        progressDialog.show();
        new Thread(){
            public void run(){
                HttpClient client = new DefaultHttpClient();
                HttpGet get = new HttpGet(url);
                HttpResponse response;
                try {
                    response = client.execute(get);
                    HttpEntity entity = response.getEntity();
                    long length = entity.getContentLength();
                    InputStream is =  entity.getContent();
                    FileOutputStream fileOutputStream = null;
                    if(is != null){
                        File file = new File(Environment.getExternalStorageDirectory(), UPDATE_SERVERAPK);
                        fileOutputStream = new FileOutputStream(file);
                        byte[] b = new byte[1024];
                        int charb = -1;
                        int count = 0;
                        while((charb = is.read(b))!=-1){
                            fileOutputStream.write(b, 0, charb);
                            count += charb;
                        }
                    }
                    fileOutputStream.flush();
                    if(fileOutputStream!=null){
                        fileOutputStream.close();
                    }
                    down();
                } catch (Exception e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }.start();
    }

    Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            progressDialog.cancel();
            updateApplication();
        }
    };

    public void down(){
        new Thread(){
            public void run(){
                Message message = handler.obtainMessage();
                handler.sendMessage(message);
            }
        }.start();
    }

    public void updateApplication()
    {
        try{
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(Uri.fromFile(new File(Environment.getExternalStorageDirectory(), UPDATE_SERVERAPK))
                                  , "application/vnd.android.package-archive");
            startActivity(intent);
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
    ////////////////////////////////////////////////////////
}
