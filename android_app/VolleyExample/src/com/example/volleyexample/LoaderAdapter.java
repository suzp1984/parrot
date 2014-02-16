package com.example.volleyexample;

import com.android.volley.toolbox.ImageLoader;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class LoaderAdapter extends BaseAdapter {

	private int mCount;
	private Context mContext;
	private String[] mUrl;
	private ImageLoader mImageLoader;
	
	public LoaderAdapter(int count, Context context, String[] url, ImageLoader imageLoader) {
		this.mCount = count;
		this.mContext = context;
		this.mUrl = url;
		this.mImageLoader = imageLoader;
		
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mCount;
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		if (position >= 0 && position < mUrl.length) {
			return mUrl[position];
		} else {
			return null;
		}
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	static class ViewHolder {
		TextView mTextView;
		ImageView mImageView;
	
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder holder;
		if (position < 0 || position >= mUrl.length) {
			return null;
		}
		
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.list_item, null);
			holder = new ViewHolder();
			holder.mTextView = (TextView) convertView.findViewById(R.id.tv_tips);
			holder.mImageView = (ImageView) convertView.findViewById(R.id.iv_image);
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}
		//holder = new ViewHolder();
		mImageLoader.get(mUrl[position], new FadeinImageListener(holder.mImageView, mContext));
		holder.mTextView.setText("--"+position+"--FLING");
		return convertView;
	}

}
