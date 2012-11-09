	static jboolean
android_server_KeyInputQueue_readEvent(JNIEnv* env, jobject clazz,
		jobject event)
{   
	gLock.lock(); 
	sp<EventHub> hub = gHub;
	if (hub == NULL) {
		hub = new EventHub; 
		gHub = hub;
	}
	gLock.unlock();

	int32_t deviceId;
	int32_t type; 
	int32_t scancode, keycode;
	uint32_t flags;
	int32_t value;
	nsecs_t when;
	bool res = hub->getEvent(&deviceId, &type, &scancode, &keycode,
			&flags, &value, &when);

	env->SetIntField(event, gInputOffsets.mDeviceId, (jint)deviceId);
	env->SetIntField(event, gInputOffsets.mType, (jint)type);
	env->SetIntField(event, gInputOffsets.mScancode, (jint)scancode);
	env->SetIntField(event, gInputOffsets.mKeycode, (jint)keycode);
	env->SetIntField(event, gInputOffsets.mFlags, (jint)flags);
	env->SetIntField(event, gInputOffsets.mValue, value);
	env->SetLongField(event, gInputOffsets.mWhen,
			(jlong)(nanoseconds_to_milliseconds(when)));

	return res;
}

