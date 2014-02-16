package com.example.volleyexample;

import android.app.Application;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.VolleyLog;
import com.android.volley.toolbox.ImageLoader;
import com.android.volley.toolbox.Volley;

public class VEApplication extends Application {
	
	private static VEApplication sVEApplication;
	private RequestQueue mRequestQueue;
	private ImageLoader mImageLoader;
	
	@Override
	public void onCreate() {
		super.onCreate();
		
		sVEApplication = this;
		mImageLoader = new ImageLoader(getRequestQueue(), new BitmapLruCache(20));
		
	}
	
	public RequestQueue getRequestQueue() {
		if (mRequestQueue == null) {
			mRequestQueue = Volley.newRequestQueue(getApplicationContext());
		}
		
		return mRequestQueue;
	}
	
	public ImageLoader getImageLoader() {
		return mImageLoader;
	}

	public <T> void addToRequestQueue(Request<T> req, String tag) {
		req.setTag(tag);
		VolleyLog.d("Adding request to queue: %s", req.getUrl());
		getRequestQueue().add(req);
	}
	
	public void cancelPendingRequests(Object tag) {
		if (mRequestQueue != null) {
			mRequestQueue.cancelAll(tag);
		}
	}
}
