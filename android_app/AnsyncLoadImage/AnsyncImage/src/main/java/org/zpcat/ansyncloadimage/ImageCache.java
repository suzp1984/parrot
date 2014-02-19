package org.zpcat.ansyncloadimage;

import android.graphics.Bitmap;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by jacobsu on 2/19/14.
 */
public class ImageCache {

    private Map<String, Bitmap> cache = new HashMap<String, Bitmap>();
    private long limit = 1000000;

    public ImageCache() {
        setLimit(Runtime.getRuntime().maxMemory()/4);
    }

    public void setLimit(long new_limit) {
        limit = new_limit;
    }

    public Bitmap get(String id) {
        if (cache.containsKey(id)) {
            return cache.get(id);
        }

        return null;
    }

    public void put(String id, Bitmap bitmap) {
        if (!cache.containsKey(id)) {
            cache.put(id, bitmap);
        }
    }

    private long getSizeInByte(Bitmap bitmap) {
        if (bitmap == null) {
            return 0;
        } else {
            return bitmap.getRowBytes() * bitmap.getHeight();
        }
    }

    public void clear() {
        cache.clear();
    }
}
