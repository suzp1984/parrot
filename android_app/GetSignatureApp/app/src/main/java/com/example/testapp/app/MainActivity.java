package com.example.testapp.app;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import java.io.ByteArrayInputStream;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;


public class MainActivity extends ActionBarActivity {
    public static final String TAG = "TestApp";
    private static final String SYSTEM_SIGNATURE = "308204d4308203bca003020102020900d20995a79c0daad6300d06092a864886f70d01010505003081a2310b3009060355040613024b52311430120603550408130b536f757468204b6f726561311330110603550407130a5375776f6e2043697479311c301a060355040a131353616d73756e6720436f72706f726174696f6e310c300a060355040b1303444d43311530130603550403130c53616d73756e6720436572743125302306092a864886f70d0109011616616e64726f69642e6f734073616d73756e672e636f6d301e170d3131303632323132323531325a170d3338313130373132323531325a3081a2310b3009060355040613024b52311430120603550408130b536f757468204b6f726561311330110603550407130a5375776f6e2043697479311c301a060355040a131353616d73756e6720436f72706f726174696f6e310c300a060355040b1303444d43311530130603550403130c53616d73756e6720436572743125302306092a864886f70d0109011616616e64726f69642e6f734073616d73756e672e636f6d30820120300d06092a864886f70d01010105000382010d00308201080282010100c986384a3e1f2fb206670e78ef232215c0d26f45a22728db99a44da11c35ac33a71fe071c4a2d6825a9b4c88b333ed96f3c5e6c666d60f3ee94c490885abcf8dc660f707aabc77ead3e2d0d8aee8108c15cd260f2e85042c28d2f292daa3c6da0c7bf2391db7841aade8fdf0c9d0defcf77124e6d2de0a9e0d2da746c3670e4ffcdc85b701bb4744861b96ff7311da3603c5a10336e55ffa34b4353eedc85f51015e1518c67e309e39f87639ff178107f109cd18411a6077f26964b6e63f8a70b9619db04306a323c1a1d23af867e19f14f570ffe573d0e3a0c2b30632aaec3173380994be1e341e3a90bd2e4b615481f46db39ea83816448ec35feb1735c1f3020103a382010b30820107301d0603551d0e04160414932c3af70b627a0c7610b5a0e7427d6cfaea3f1e3081d70603551d230481cf3081cc8014932c3af70b627a0c7610b5a0e7427d6cfaea3f1ea181a8a481a53081a2310b3009060355040613024b52311430120603550408130b536f757468204b6f726561311330110603550407130a5375776f6e2043697479311c301a060355040a131353616d73756e6720436f72706f726174696f6e310c300a060355040b1303444d43311530130603550403130c53616d73756e6720436572743125302306092a864886f70d0109011616616e64726f69642e6f734073616d73756e672e636f6d820900d20995a79c0daad6300c0603551d13040530030101ff300d06092a864886f70d01010505000382010100329601fe40e036a4a86cc5d49dd8c1b5415998e72637538b0d430369ac51530f63aace8c019a1a66616a2f1bb2c5fabd6f313261f380e3471623f053d9e3c53f5fd6d1965d7b000e4dc244c1b27e2fe9a323ff077f52c4675e86247aa801187137e30c9bbf01c567a4299db4bf0b25b7d7107a7b81ee102f72ff47950164e26752e114c42f8b9d2a42e7308897ec640ea1924ed13abbe9d120912b62f4926493a86db94c0b46f44c6161d58c2f648164890c512dfb28d42c855bf470dbee2dab6960cad04e81f71525ded46cdd0f359f99c460db9f007d96ce83b4b218ac2d82c48f12608d469733f05a3375594669ccbf8a495544d6c5701e9369c08c810158";


    private ArrayList<Signature> mIboxpaySignatures = new ArrayList<Signature>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final PackageManager pm = getPackageManager();
        List<PackageInfo> pkgs = pm.getInstalledPackages(PackageManager.GET_SIGNATURES |
                                                        PackageManager.GET_ACTIVITIES |
                                                        PackageManager.GET_SERVICES |
                                                        PackageManager.GET_META_DATA);
        // print all package info with its signatures and certificate
        /*

        for(PackageInfo info : pkgs) {
            //info.
            //Log.w("TestApp", String.valueOf(info.signatures));
            Signature[] sigs = info.signatures;
            for (Signature sig : sigs) {
                Log.d(TAG, info.packageName + ": " + sig.toCharsString());
                //Log.d(TAG, sig.getPublicKey());
                try {
                    final CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
                    final ByteArrayInputStream bais = new ByteArrayInputStream(sig.toByteArray());
                    final Certificate cert = certFactory.generateCertificate(bais);
                    Log.d(TAG, info.packageName + " cert.getPublicKey() = " + cert.getPublicKey());
                    Log.d(TAG, info.packageName + " cert.getEncoded() = " + cert.getEncoded());
                    Log.d(TAG, info.packageName + " cert.toString() = " + cert.toString());
                    X509Certificate cert509 = (X509Certificate) cert;
                    Log.d(TAG, info.packageName + " cert issuer = " + cert509.getIssuerDN());
                } catch (CertificateException e) {
                    e.printStackTrace();
                }
            }
        } */

        try {
            PackageInfo settings = pm.getPackageInfo("com.android.settings", PackageManager.GET_SIGNATURES|PackageManager.GET_ACTIVITIES
                                                                            | PackageManager.GET_SERVICES | PackageManager.GET_PERMISSIONS);
            PackageInfo phone = pm.getPackageInfo("com.android.phone", PackageManager.GET_SIGNATURES | PackageManager.GET_ACTIVITIES
                                                                       | PackageManager.GET_SERVICES);
            PackageInfo solitons = pm.getPackageInfo("com.solitonmusic.solitons", PackageManager.GET_SIGNATURES | PackageManager.GET_ACTIVITIES
                                                                            | PackageManager.GET_SERVICES);
            PackageInfo cashbox = pm.getPackageInfo("com.iboxpay.minicashbox", PackageManager.GET_SIGNATURES | PackageManager.GET_ACTIVITIES
                                                                            | PackageManager.GET_SERVICES);

            Signature[] settings_sigs = settings.signatures;
            Signature[] phone_sigs = phone.signatures;
            Signature[] soliton_sigs = solitons.signatures;
            Signature[] cashbox_sigs = cashbox.signatures;

            Log.w(TAG, "cashbox signatures size is " + cashbox_sigs.length);

            Signature settings_sigs_1 = settings_sigs[0];
            Signature phone_sigs_1 = phone_sigs[0];
            Signature soliton_sigs_1 = soliton_sigs[0];
            Signature system_sig = new Signature(SYSTEM_SIGNATURE);
            Signature cashbox_sig = cashbox_sigs[0];

            /*
            for (Signature sig : settings_sigs) {
                Log.d(TAG, settings.packageName + ": " + sig.toString());

            } */

            Log.w(TAG, "System signature : " + system_sig.toCharsString());
            Log.w(TAG, settings.packageName + ": " + settings_sigs_1.toCharsString());
            Log.w(TAG, phone.packageName + ": " + phone_sigs_1.toCharsString());
            Log.w(TAG, solitons.packageName + ": " + soliton_sigs_1.toCharsString());
            Log.w(TAG, cashbox.packageName + ": " + cashbox_sig.toCharsString());

            if (system_sig.equals(settings_sigs_1)) {
                Log.w(TAG, "============= system signature == settings signature=====");
            } else {
                Log.w(TAG, "************** system signature != settings signature*******");
            }

            if (settings_sigs_1.equals(soliton_sigs_1)) {
                Log.w(TAG, "============ settings signature == solitons signature =======");
            } else {
                Log.w(TAG, "************* settings signature != solitons signature ********");
            }

            if (settings_sigs_1.equals(phone_sigs_1)) {
                Log.w(TAG, "===========Setting sig = Phone sig ===========");
            } else {
                Log.w(TAG, "**********Settings sig != Phone sig ******************");
            }

            final CertificateFactory certfactory = CertificateFactory.getInstance("X.509");

            final ByteArrayInputStream settings_in = new ByteArrayInputStream(settings_sigs_1.toByteArray());
            final X509Certificate settings_cert = (X509Certificate) certfactory.generateCertificate(settings_in);

            final ByteArrayInputStream phone_in = new ByteArrayInputStream(phone_sigs_1.toByteArray());
            final X509Certificate phone_cert = (X509Certificate) certfactory.generateCertificate(phone_in);

            final ByteArrayInputStream soliton_in = new ByteArrayInputStream(soliton_sigs_1.toByteArray());
            final X509Certificate solitons_cert = (X509Certificate) certfactory.generateCertificate(soliton_in);

            final ByteArrayInputStream cashbox_in = new ByteArrayInputStream(cashbox_sig.toByteArray());
            final X509Certificate cashbox_cert = (X509Certificate) certfactory.generateCertificate(cashbox_in);

            if (settings_cert.equals(solitons_cert)) {
                Log.w(TAG, "=========== settings cert == soliton cert =============");
            } else {
                Log.w(TAG, "************** settings cert != solition cert *************");
            }

            if (settings_cert.equals(phone_cert)) {
                Log.w(TAG, "============== Settings cert == Phone cert =========");
            } else {
                Log.w(TAG, "***************** Settings cert != Phone cert *********");
            }

            Log.w(TAG, settings.packageName + ": " + settings_cert.toString());
            Log.w(TAG, phone.packageName + ": " + phone_cert.toString());
            Log.w(TAG, solitons.packageName + ": " + solitons_cert.toString());
            Log.w(TAG, cashbox.packageName + ": " + cashbox_cert.toString());

        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        } catch (CertificateException e) {
            e.printStackTrace();
        }
    }


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
}
