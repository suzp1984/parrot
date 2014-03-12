package com.zpcat.remoteservicesample.app;

interface IRemoteServiceCallback {
    /**
     * Called when the service has a new value for you.
     */
    void showMessage(String msg);
}
