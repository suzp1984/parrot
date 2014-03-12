package org.zpcat.localservicesample.app;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Binder;
import android.os.IBinder;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteCallbackList;
import android.support.v4.app.NotificationCompat;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.logging.LogRecord;

/**
 * Created by zxsu on 14-3-12.
 *
 * This is a Service running in the process of this application.
 * I tried to bind to this Service, then register a callback func
 * and can send message to activity
 * by callback function
 */
public class LocalService extends Service {

    private int NOTIFICATION = R.string.local_service_started;
    private NotificationManager mNM;

    private LocalServiceCallback mCB = null;

    public class LocalBinder extends Binder {
        LocalService getService() {
            return LocalService.this;
        }
    }

    private int mValue = 0;
    private static final int REPORT_MSG = 1;

    private final Handler mHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case REPORT_MSG:
                    int value = ++mValue;

                    if (mCB != null) {
                        mCB.showMsg(String.valueOf(value));
                    }
                    sendMessageDelayed(obtainMessage(REPORT_MSG), 1*1000);
            }
        }


    };

    public void registerCB(LocalServiceCallback cb) {
        mCB = cb;
    }

    private final IBinder mBinder = new LocalBinder();

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {
        mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        showNotification();
        mHandler.sendEmptyMessage(REPORT_MSG);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return 0;
    }

    @Override
    public void onDestroy() {
        mNM.cancel(NOTIFICATION);
        Toast.makeText(this, R.string.local_service_stoped, Toast.LENGTH_SHORT).show();
        mHandler.removeMessages(REPORT_MSG);
    }

    private void showNotification() {
        CharSequence text = getText(R.string.local_service_started);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.ic_launcher)
                .setContentTitle(text)
                .setWhen(System.currentTimeMillis());

        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, MainActivity.class), 0);
        builder.setContentIntent(contentIntent);

        mNM.notify(NOTIFICATION, builder.build());
    }

    public interface LocalServiceCallback {
        public void showMsg(String msg);
    }

}
