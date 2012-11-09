#include "delayed_typer_command.h"
#include "sleep_command.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	ActiveObjectEngine* mEng;
	char c;
	int stop;
	long delay;
	CommandInterface* mSleepCmd;
} PrivInfo;

static void delay_and_repeat(CommandInterface* thiz) 
{
	return_if_fail(thiz != NULL);
	DECLES_PRIV(priv, thiz);

	priv->mSleepCmd = sleep_command_create(priv->delay, priv->mEng, thiz);
	//TODO add a sleep command
	active_object_engine_add_command(priv->mEng, priv->mSleepCmd);
}

static Ret delayed_typer_command_execute(CommandInterface* thiz)
{
	DECLES_PRIV(priv, thiz);

	printf("%c\n", priv->c);
	if (priv->mSleepCmd) {
		command_destroy(priv->mSleepCmd);
	}
		
	if (!priv->stop) {
		delay_and_repeat(thiz);
	}
}

static void delayed_typer_command_destroy(CommandInterface* thiz)
{
	DECLES_PRIV(priv, thiz);

	if (priv->mSleepCmd) {
		command_destroy(priv->mSleepCmd);
	}

	SAFE_FREE(thiz);
}

CommandInterface* delayed_typer_command_create(long milliseconds, char c, ActiveObjectEngine* eng)
{
	CommandInterface* thiz = (CommandInterface*) malloc(sizeof(CommandInterface) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);

		thiz->execute = delayed_typer_command_execute;
		thiz->destroy = delayed_typer_command_destroy;

		priv->mEng = eng;
		priv->delay = milliseconds;
		priv->c = c;
		priv->stop = 0;
	}

	return thiz;
}


#ifdef DELAYED_TYPER_TEST

int main(int argc, char* argv[])
{
	ActiveObjectEngine* eng = active_object_engine_create();
	
	CommandInterface* dtyper_a = delayed_typer_command_create(200, 'a', eng);
	CommandInterface* dtyper_b = delayed_typer_command_create(400, 'b', eng);
	CommandInterface* dtyper_c = delayed_typer_command_create(600, 'c', eng);
	CommandInterface* dtyper_d = delayed_typer_command_create(700, 'd', eng);

	active_object_engine_add_command(eng, dtyper_a);
	active_object_engine_add_command(eng, dtyper_b);
	active_object_engine_add_command(eng, dtyper_c);
	active_object_engine_add_command(eng, dtyper_d);

	active_object_engine_run(eng);

	command_destroy(dtyper_a);
	command_destroy(dtyper_b);
	command_destroy(dtyper_c);
	command_destroy(dtyper_d);

	active_object_engine_destroy(eng);
	return 0;
}

#endif // DELAYED_TYPER_TEST
