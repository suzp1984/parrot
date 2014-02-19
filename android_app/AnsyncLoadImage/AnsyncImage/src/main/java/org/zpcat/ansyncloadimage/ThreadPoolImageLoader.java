package org.zpcat.ansyncloadimage;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.BaseAdapter;
import android.widget.ImageView;

import java.io.InputStream;
import java.io.OutputStream;
import java.lang.ref.WeakReference;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by jacobsu on 2/19/14.
 */
public class ThreadPoolImageLoader implements ImageLoader {

    private Context mContext;
    private BaseAdapter mAdapter;
    private ExecutorService mExecuteService;
    private ImageCache mImageCache;

    public ThreadPoolImageLoader(Context context, BaseAdapter baseAdapter) {
        mContext = context;
        mAdapter = baseAdapter;
        mExecuteService = Executors.newFixedThreadPool(5);
        mImageCache = new ImageCache();
    }

    public void displayImage(String url, ImageView image) {
        Bitmap bitmap = mImageCache.get(url);
        if (bitmap == null) {
            mExecuteService.submit(new LoaderRunner(url, image));

        } else {
            image.setImageBitmap(bitmap);
        }

    }


    class LoaderRunner implements Runnable {

        private String mUrl = null;
        //private ImageView mImage = null;
        private WeakReference<ImageView> imageViewWeakReference;

        public LoaderRunner(String url, ImageView image) {
            mUrl = url;
            imageViewWeakReference = new WeakReference<ImageView>(image);
        }

        @Override
        public void run() {
            try {
                URL imageurl = new URL(mUrl);
                HttpURLConnection conn = (HttpURLConnection)imageurl.openConnection();
                conn.connect();
                InputStream is = conn.getInputStream();
                Bitmap bp = BitmapFactory.decodeStream(is);
                is.close();
                mImageCache.put(mUrl, bp);
                ImageView image = imageViewWeakReference.get();
                image.setImageBitmap(bp);
                mAdapter.notifyDataSetChanged();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
