#include "sleep_command.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	long sleeptime;
	ActiveObjectEngine* mEng;
	CommandInterface* mWakupCmd;
	long starttime;
	int started;
} PrivInfo;

double ms_uptime(void)
{
	FILE* in;
	char tmp[256];
	double retval = 0;

	in = fopen("/proc/uptime", "r");
	if (in != NULL) {
		fgets(tmp, sizeof(tmp), in);
		retval = atof(tmp);
		fclose(in);
	}

	return retval * 1000;
}

static Ret sleep_command_execute(CommandInterface* thiz) 
{
	//return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	DECLES_PRIV(priv, thiz);

	long current = (long)ms_uptime();
	if (priv->started == -1) {
		priv->started = 1;
		priv->starttime = current;
		//printf("%s: first start\n", __func__);
		active_object_engine_add_command(priv->mEng, thiz);
	} else if (current - priv->starttime < priv->sleeptime) {
		active_object_engine_add_command(priv->mEng, thiz);
		//printf("%s: add sleep command again\n", __func__);
	} else {
		active_object_engine_add_command(priv->mEng, priv->mWakupCmd);
		//printf("%s: add wakeup command \n", __func__);
	}

	return RET_OK;
}

static void sleep_command_destroy(CommandInterface* thiz)
{
	//return_if_fail(thiz != NULL);

//	SAFE_FREE(priv);
	SAFE_FREE(thiz);
}

CommandInterface* sleep_command_create(int milliseconds, ActiveObjectEngine* eng, 
		CommandInterface* wakeupCommand)
{
	return_val_if_fail(eng != NULL, NULL);

	CommandInterface* thiz = (CommandInterface*) malloc(sizeof(CommandInterface) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);

		thiz->execute = sleep_command_execute;
		thiz->destroy = sleep_command_destroy;

		priv->sleeptime = milliseconds;
		priv->mEng = eng;
		priv->mWakupCmd = wakeupCommand;
		priv->started = -1;
	}

	return thiz;
}

#ifdef SLEEP_COMMAND_TEST

#include "type_command.h"
#include "active_object_engine.h"
#include "sleep_command.h"
#include "logger.h"

#define MSG "helle world!"
int main(int argc, char* argv[])
{
	LOG_INIT("user.log");
	ActiveObjectEngine* eng = active_object_engine_create();
	LOG_INFO("after create active object engine");
	CommandInterface* wakup_cmd = type_command_create(MSG);
	CommandInterface* sleep_cmd = sleep_command_create(1000, eng, wakup_cmd);

	active_object_engine_add_command(eng, sleep_cmd);
	active_object_engine_run(eng);

	command_destroy(wakup_cmd);
	command_destroy(sleep_cmd);
	active_object_engine_destroy(eng);

	LOG_FINISH();

	return 0;
}

#endif //SLEEP_COMMAND_TEST
