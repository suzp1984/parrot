#include "echoloop_cmd.h"

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include <string.h>

typedef struct {
	int thread_run;
	int frames;
	snd_pcm_t* playback_handle;
	snd_pcm_t* capture_handle;
} PrivInfo;

#define PCM_DEFAULT "default"
#define RATE_SAMPLE 88200
#define DEFAULT_FRAMES 8

static void echoloop_cmd_set_capture_handle(CmdInterface* thiz)
{
	DECLES_PRIV(priv, thiz);
	int rc;
	unsigned int val;
	int dir;

	snd_pcm_hw_params_t* params;
	snd_pcm_uframes_t frames;

	/* open PCM device for capture */
	rc = snd_pcm_open(&priv->capture_handle, PCM_DEFAULT, 
			SND_PCM_STREAM_CAPTURE, 0);

	if (rc < 0) {
		LOGE("%s: unable to open pcm device: %s", __func__,
				snd_strerror(rc));
		return;
	}

	/* Allocate a hardware parameters object */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it with default value */
	snd_pcm_hw_params_any(priv->capture_handle, params);

	/* Interleaved mode */
	snd_pcm_hw_params_set_access(priv->capture_handle, params,
			SND_PCM_ACCESS_RW_INTERLEAVED);

	/*signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(priv->capture_handle, params,
			SND_PCM_FORMAT_S16_LE);

	/*Two channels (stereo) */
	snd_pcm_hw_params_set_channels(priv->capture_handle, params, 2);

	/* 44100bits/seconds sampling rate (CD quality) */
	val = RATE_SAMPLE;
	snd_pcm_hw_params_set_rate_near(priv->capture_handle, params,
			&val, &dir);

	/* Set period size to 32 frames */
	frames = DEFAULT_FRAMES;
	snd_pcm_hw_params_set_period_size_near(priv->capture_handle,
			params, &frames, &dir);


	rc = snd_pcm_hw_params(priv->capture_handle, params);
	if (rc < 0) {
		LOGE("%s: unable to set hw parameters: %s", __func__, snd_strerror(rc));
	}
}

static void echoloop_cmd_set_playback_handle(CmdInterface* thiz)
{
	DECLES_PRIV(priv, thiz);
	int rc;
	unsigned int val;
	int dir;

	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;

	/* open PCM driver for playback */
	rc = snd_pcm_open(&priv->playback_handle, PCM_DEFAULT,
			SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0) {
		LOGE("%s: unable to open pcm device: %s", __func__,
				snd_strerror(rc));
		return;
	}

	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(priv->playback_handle, params);

	/* Interleaved mode */
	snd_pcm_hw_params_set_access(priv->playback_handle, params,
			SND_PCM_ACCESS_RW_INTERLEAVED);

	/*signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(priv->playback_handle, params,
			SND_PCM_FORMAT_S16_LE);

	/*Two channels (stereo) */
	snd_pcm_hw_params_set_channels(priv->playback_handle, params, 2);

	/* 44100bits/seconds sampling rate (CD quality) */
	val = RATE_SAMPLE;
	snd_pcm_hw_params_set_rate_near(priv->playback_handle, params,
			&val, &dir);

	/* Set period size to 32 frames */
	frames = DEFAULT_FRAMES;
	snd_pcm_hw_params_set_period_size_near(priv->playback_handle,
			params, &frames, &dir);

	/* write parameters to the driver */
	rc = snd_pcm_hw_params(priv->playback_handle, params);
	if (rc < 0) {
		LOGE("%s: unable to set hw parameters: %s", __func__, snd_strerror(rc));
		return;
	}

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	priv->frames = (int)frames;

	return;
}

static void* echoloop_cmd_thread_run(void* ctx)
{
	CmdInterface* thiz = (CmdInterface*)ctx;
	DECLES_PRIV(priv, thiz);
	char* buffer;
	int size;
	int rc;

	LOGE("%s: set alsa switch done", __func__);

	/* set capture handle */
	echoloop_cmd_set_capture_handle(thiz);

	/* set playback handle */
	echoloop_cmd_set_playback_handle(thiz);

	size = priv->frames * 4; // 2 bytes/sample, 2 channels
	buffer = (char*)malloc(size);
	LOGE("%s: frames = %d; size = %d", __func__, priv->frames, size);

	while (priv->thread_run == 1) {
		memset(buffer, 0, size);

		rc = snd_pcm_readi(priv->capture_handle, buffer, priv->frames);
		if (rc == -EPIPE) {
			LOGE("%s: capture overrun occurred", __func__);
			snd_pcm_prepare(priv->capture_handle);
		}

		while((rc = snd_pcm_writei(priv->playback_handle, buffer, priv->frames)) < 0) {
			usleep(1000);
			if (rc == -EPIPE) {
				LOGE("%s: playback overrun occurred", __func__);
				snd_pcm_prepare(priv->playback_handle);
			} else if (rc < 0) {
				LOGE("%s: error from writei: %s", __func__,
						snd_strerror(rc));
			}
		}
	}

	rc = snd_pcm_drain(priv->capture_handle);
	rc = snd_pcm_close(priv->capture_handle);
	rc = snd_pcm_drain(priv->playback_handle);
	rc = snd_pcm_close(priv->playback_handle);

	free(buffer);

	return NULL;
}

static int echoloop_cmd_execute(CmdInterface* thiz, void* ctx)
{
	DECLES_PRIV(priv, thiz);
	pthread_t t;
	
	priv->thread_run = 1;
	pthread_create(&t, NULL, echoloop_cmd_thread_run, (void*)thiz);
	
	while(1) {
		sleep(5);
	}

	priv->thread_run = 0;
	pthread_join(t, NULL);
	// done

	return 0;
}

static void echoloop_cmd_destroy(CmdInterface* thiz)
{
	SAFE_FREE(thiz);
}

CmdInterface* echoloop_cmd_create(void)
{
	CmdInterface* thiz = (CmdInterface*) malloc(sizeof(CmdInterface) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);
		thiz->execute = echoloop_cmd_execute;
		thiz->destroy = echoloop_cmd_destroy;

		thiz->cmd = 1;
		priv->thread_run = 0;
		priv->frames = 0;
	}

	return thiz;
}

#ifdef ECHOLOOP_CMD_TEST

int main(int argc, char* argv[])
{
	CmdInterface* thiz = echoloop_cmd_create();

	cmd_interface_execute(thiz, NULL);

	cmd_interface_destroy(thiz);
	return 0;
}

#endif // ECHOLOOP_CMD_TEST
