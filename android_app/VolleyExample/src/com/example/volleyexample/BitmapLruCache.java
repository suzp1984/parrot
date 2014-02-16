package com.example.volleyexample;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.graphics.Bitmap;
import android.os.Build;
import android.util.LruCache;

import com.android.volley.toolbox.ImageLoader.ImageCache;

@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR1)
@SuppressLint("NewApi")
public class BitmapLruCache extends LruCache<String, Bitmap> implements
		ImageCache {

	public BitmapLruCache(int maxSize) {
		super(maxSize);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected int sizeOf(String key, Bitmap value) {
		return value.getRowBytes() * value.getHeight();
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR1)
	@SuppressLint("NewApi")
	@Override
	public Bitmap getBitmap(String url) {
		// TODO Auto-generated method stub
		return get(url);
	}

	@Override
	public void putBitmap(String url, Bitmap bitmap) {
		// TODO Auto-generated method stub
		put(url, bitmap);

	}

}
