package com.zpcat.remoteservicesample.app;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity extends ActionBarActivity {

    private Button mBindRemote = null;
    private Button mUnbindRemote = null;
    private Button mBindMessenger = null;
    private Button mUnbindMessenger = null;

    private TextView mRemoteMsg = null;
    private TextView mMessengerMsg = null;

    private IRemoteService mRemoteService = null;
    private Messenger mMessengerService = null;

    private boolean mIsRemoteBind = false;
    private boolean mIsMsgBind = false;

    static final int SHOW_REMOTE_MSG = 5;

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MessengerService.MSG_SET_VALUE:
                    mMessengerMsg.setText(String.valueOf(msg.arg1));
                    break;
                case SHOW_REMOTE_MSG:
                    if (mRemoteMsg != null) {
                        mRemoteMsg.setText((String)msg.obj);
                    }
                    break;
            }
        }
    };

    private ServiceConnection mRemoteConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mRemoteService = IRemoteService.Stub.asInterface(service);
            try {
                mRemoteService.registerCallback(mCallback);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            try {
                mRemoteService.unregisterCallback(mCallback);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
            mRemoteService = null;
        }
    };

    private ServiceConnection mMessengerConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mMessengerService = new Messenger(service);

            try {
                Message msg = Message.obtain(null,
                        MessengerService.MSG_REGISTER_CLIENT);
                msg.replyTo = mMessenger;
                mMessengerService.send(msg);

            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mMessengerService = null;
        }
    };

    private final Messenger mMessenger = new Messenger(mHandler);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mBindRemote = (Button) findViewById(R.id.bind_remote);
        mUnbindRemote = (Button) findViewById(R.id.unbind_remote);

        mBindMessenger = (Button) findViewById(R.id.bind_messenger);
        mUnbindMessenger = (Button) findViewById(R.id.unbind_messenger);

        mRemoteMsg = (TextView) findViewById(R.id.remote_msg);
        mMessengerMsg = (TextView) findViewById(R.id.messenger_msg);

        mBindRemote.setOnClickListener(mBindRemoteListener);
        mUnbindRemote.setOnClickListener(mUnbindRemoteListenr);

        mBindMessenger.setOnClickListener(mBinderMessengerListener);
        mUnbindMessenger.setOnClickListener(mUnbinderMessengerListener);
    }

    private void onBindMessengerService() {
        bindService(new Intent(this, MessengerService.class),
                mMessengerConnection, Context.BIND_AUTO_CREATE);
        mIsMsgBind = true;

    }

    private void onUnbindMessengerService() {
        if (mIsMsgBind) {
            if (mMessengerService != null) {
                try {
                    Message msg = Message.obtain(null,
                            MessengerService.MSG_UNREGISTER_CLIENT);
                    msg.replyTo = mMessenger;
                    mMessengerService.send(msg);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }

            unbindService(mMessengerConnection);
            mIsMsgBind = false;
        }
    }

    View.OnClickListener mBinderMessengerListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            onBindMessengerService();
        }
    };

    View.OnClickListener mUnbinderMessengerListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            onUnbindMessengerService();
        }
    };

    private void onBindRemoteService() {
        bindService(new Intent(IRemoteService.class.getName()),
                mRemoteConnection, Context.BIND_AUTO_CREATE);
        mIsRemoteBind = true;
    }

    private void onUnbindRemoteService() {
        if (mIsRemoteBind) {
            if (mRemoteService != null) {
                try {
                    mRemoteService.unregisterCallback(mCallback);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }

            unbindService(mRemoteConnection);
            mIsRemoteBind = false;
        }
    }

    View.OnClickListener mBindRemoteListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            onBindRemoteService();
        }
    };

    View.OnClickListener mUnbindRemoteListenr = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            onUnbindRemoteService();
        }
    };


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private IRemoteServiceCallback mCallback = new IRemoteServiceCallback.Stub() {
        @Override
        public void showMessage(String msg) {
            mHandler.sendMessage(mHandler.obtainMessage(SHOW_REMOTE_MSG, msg));
            //mRemoteMsg.setText(msg);
        }

    };
}
