package com.zpcat.remoteservicesample.app;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.support.v4.app.NotificationCompat;
import android.widget.Toast;

/**
 * Created by jacobsu on 3/12/14.
 */
public class RemoteService extends Service {

    private int NOTIFICATION = R.string.remote_service_started;

    private RemoteCallbackList<IRemoteServiceCallback> mRemoteCallbacks =
            new RemoteCallbackList<IRemoteServiceCallback>();

    private NotificationManager mNM = null;
    private static final int REPORT_MSG = 1;
    private int mValue = 0;

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case REPORT_MSG: {
                    int value = ++mValue;
                    final int N = mRemoteCallbacks.beginBroadcast();
                    for (int i = 0; i < N; i++) {
                        try {
                            mRemoteCallbacks.getBroadcastItem(i).showMessage(String.valueOf(value));
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                    mRemoteCallbacks.finishBroadcast();

                    sendMessageDelayed(obtainMessage(REPORT_MSG), 1*1000);
                } break;
                default:
                    super.handleMessage(msg);
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        return mRemoteService;
    }

    @Override
    public void onCreate() {
        mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        showNotification();
        mHandler.sendEmptyMessage(REPORT_MSG);
    }

    @Override
    public void onDestroy() {
        mNM.cancel(NOTIFICATION);
        Toast.makeText(this, R.string.remote_service_stoped, Toast.LENGTH_SHORT).show();
        mRemoteCallbacks.kill();
        mHandler.removeMessages(REPORT_MSG);
    }

    private void showNotification() {
        CharSequence text = getText(R.string.remote_service_started);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this)
                .setSmallIcon(R.drawable.ic_launcher)
                .setContentTitle(text)
                .setWhen(System.currentTimeMillis());

        PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, MainActivity.class), 0);
        builder.setContentIntent(contentIntent);

        mNM.notify(NOTIFICATION, builder.build());
    }

    private IRemoteService.Stub mRemoteService = new IRemoteService.Stub() {
        public void registerCallback(IRemoteServiceCallback cb) {
            if (cb != null) {
                mRemoteCallbacks.register(cb);
            }
        }

        public void unregisterCallback(IRemoteServiceCallback cb) {
            if (cb != null) {
                mRemoteCallbacks.unregister(cb);
            }
        }
    };
}
