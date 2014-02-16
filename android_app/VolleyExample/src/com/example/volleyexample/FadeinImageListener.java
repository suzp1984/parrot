package com.example.volleyexample;

import java.lang.ref.WeakReference;

import android.content.Context;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.ImageLoader;
import com.android.volley.toolbox.ImageLoader.ImageContainer;

public class FadeinImageListener implements ImageLoader.ImageListener {

	private WeakReference<ImageView> mImageView;
	private Context mContext;
	
	public FadeinImageListener (ImageView image, Context context) {
		mImageView = new WeakReference<ImageView>(image);
		mContext = context;
	}
	
	@Override
	public void onErrorResponse(VolleyError error) {
		// TODO Auto-generated method stub
		if (mImageView.get() != null) {
			mImageView.get().setImageResource(R.drawable.ic_launcher);
		}
		
	}

	@Override
	public void onResponse(ImageContainer response, boolean arg1) {
		// TODO Auto-generated method stub
		if (mImageView.get() != null) {
			ImageView image = mImageView.get();
			if (response.getBitmap() != null) {
				image.startAnimation(AnimationUtils.loadAnimation(mContext, R.anim.fade_in));
				image.setImageBitmap(response.getBitmap());
			} else {
				image.setImageResource(R.drawable.ic_launcher);
			}
		}
		
	}

}
