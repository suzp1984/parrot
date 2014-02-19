package org.zpcat.ansyncloadimage;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.support.v7.app.ActionBarImplJB;
import android.widget.BaseAdapter;
import android.widget.ImageView;

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * Created by jacobsu on 2/19/14.
 */
public class AsyncTaskImageLoader implements ImageLoader {

    private Context mContext = null;
    private ImageCache mImageCache = null;
    private BaseAdapter mAdapter = null;

    public AsyncTaskImageLoader(Context context, BaseAdapter adapter) {
        mContext = context;
        mImageCache = new ImageCache();
        mAdapter = adapter;
    }

    public void displayImage(String url, ImageView imageView) {
        Bitmap bitmap = null;
        bitmap = mImageCache.get(url);

        if (bitmap == null) {
            DownloadFileTask asyncTask = new DownloadFileTask(imageView);
            asyncTask.execute(url);
            return;
        }

        imageView.setImageBitmap(bitmap);
    }

    private Bitmap downloadImage(String url) {
        return null;
    }

    private class DownloadFileTask extends AsyncTask<String, Void, Bitmap> {

        private final WeakReference<ImageView> imageViewWeakReference;

        public DownloadFileTask(ImageView imageView) {
            imageViewWeakReference = new WeakReference<ImageView>(imageView);
        }

        @Override
        protected Bitmap doInBackground(String... urls) {
            URL url = null;
            try {
                url = new URL(urls[0]);
                HttpURLConnection connection;
                connection = (HttpURLConnection)url.openConnection();
                connection.connect();
                InputStream input = connection.getInputStream();
                Bitmap bmp = BitmapFactory.decodeStream(input);
                input.close();
                mImageCache.put(urls[0], bmp);
            } catch (Exception e) {
                e.printStackTrace();
            }
            //HttpURLConnection connection = url.openConnection();
            return null;
        }

        @Override
        protected void onPostExecute(Bitmap bitmap) {
            if(imageViewWeakReference != null) {
                ImageView imageView = imageViewWeakReference.get();
                imageView.setImageBitmap(bitmap);
                mAdapter.notifyDataSetChanged();
            }

        }


    }
}
