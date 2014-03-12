package org.zpcat.localservicesample.app;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity extends ActionBarActivity {

    private TextView mMsg = null;
    private Button mBindButton = null;
    private Button mUnBindButton = null;
    private boolean mIsBond = false;
    private LocalService mBoundService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mMsg = (TextView)findViewById(R.id.msg_show);
        mBindButton = (Button)findViewById(R.id.bind_service);
        mUnBindButton = (Button) findViewById(R.id.unbind_service);

        mBindButton.setOnClickListener(mBindListener);
        mUnBindButton.setOnClickListener(mUnBindListener);
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mBoundService = ((LocalService.LocalBinder)service).getService();
            mBoundService.registerCB(mServiceCB);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mBoundService = null;
        }
    };

    private void doBindService() {
        bindService(new Intent(this, LocalService.class), mConnection, Context.BIND_AUTO_CREATE);
        mIsBond = true;
    }

    private void doUnBindService() {
        if (mIsBond) {
            unbindService(mConnection);
            mIsBond = false;
        }
    }

    private View.OnClickListener mBindListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            doBindService();
        }
    };

    private View.OnClickListener mUnBindListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            doUnBindService();
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

    private ServiceCB mServiceCB = new ServiceCB();

    public class ServiceCB implements LocalService.LocalServiceCallback {

        @Override
        public void showMsg(String msg) {
            mMsg.setText(msg);
        }
    }
}
