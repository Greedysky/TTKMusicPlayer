package org.greedysky.ttkmobile;

import java.util.Timer;
import java.util.TimerTask;
import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;

public class SplashScreen extends Activity
{
    private Timer timer = new Timer();
    private TimerTask task = new TimerTask() {
        @Override
        public void run() {
            Intent intent = new Intent();
            intent.setClass(SplashScreen.this, TTKMobile.class);
            startActivity(intent);
            SplashScreen.this.finish();
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        timer.schedule(task, 1500);
    }
}
