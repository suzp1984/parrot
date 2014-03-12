package com.zpcat.remoteservicesample.app;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v4.app.NotificationCompat;

import java.util.ArrayList;

/**
 * Created by jacobsu on 3/12/14.
 */
public class MessengerService extends Service {


    private int NOTIFICATION = R.string.messenger_service_started;
    private NotificationManager mNM;

    ArrayList<Messenger> mClients = new ArrayList<Messenger>();

    static final int MSG_REGISTER_CLIENT = 1;
    static final int MSG_UNREGISTER_CLIENT = 2;
    static final int MSG_SET_VALUE = 3;

    private int mValue = 0;

    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_REGISTER_CLIENT:
                    mClients.add(msg.replyTo);
                    break;
                case MSG_UNREGISTER_CLIENT:
                    mClients.remove(msg.replyTo);
                    break;
                case MSG_SET_VALUE:
                    int value = ++mValue;
                    for (int i = mClients.size() - 1; i >= 0; i--) {
                        try {
                            mClients.get(i).send(Message.obtain(null,
                                    MSG_SET_VALUE, mValue, 0));
                        } catch (RemoteException e) {
                            mClients.remove(i);
                        }
                    }
                    sendMessageDelayed(obtainMessage(MSG_SET_VALUE), 1*1000);
                    break;
                default:
                    super.handleMessage(msg);

            }
        }
    }

    private IncomingHandler mHandler = new IncomingHandler();

    private Messenger mMessenger = new Messenger(mHandler);

    @Override
    public void onCreate() {
        mNM = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        showNotification();
        mHandler.sendEmptyMessage(MSG_SET_VALUE);
    }

    @Override
    public void onDestroy() {
        mNM.cancel(R.string.messenger_service_started);

    }
    @Override
    public IBinder onBind(Intent intent) {
        return mMessenger.getBinder();
    }

    private void showNotification() {
        CharSequence text = getText(R.string.messenger_service_started);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.ic_launcher)
                .setContentTitle(text)
                .setWhen(System.currentTimeMillis());

        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, MainActivity.class), 0);
        builder.setContentIntent(contentIntent);

        mNM.notify(NOTIFICATION, builder.build());
    }
}
