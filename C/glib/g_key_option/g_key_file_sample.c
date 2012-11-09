#include <glib.h>
#include <error.h>
#include <stdio.h>

static GKeyFile* load_config(const char* file)
{
	GError* err = NULL;
	GKeyFile* keyfile;

	keyfile = g_key_file_new();
	g_key_file_set_list_separator(keyfile, ',');

	if (!g_key_file_load_from_file(keyfile, file, 0, &err)) {
		error("Parsing %s failed: %s", file, err->message);
		g_error_free(err);
		g_key_file_free(keyfile);
		return NULL;
	}

	return keyfile;
}

int main(int argc, char* argv[])
{
	GKeyFile* config;
	GError* err = NULL;
	int val;
	char* str;
	gboolean boolean;

	config = load_config("./main.conf");
	if(!config) return;

	val = g_key_file_get_integer(config, "General", "DiscoverableTimeout", &err);

	if(err) {
		printf("%s", err->message);
		g_clear_error(&err);
	} else {
		printf("in [General] DiscoverableTimeout is %d \n", val);
	}

	str = g_key_file_get_string(config, "General", "Name", &err);
	if(err) {
		printf("%s", err->message);
		g_clear_error(&err);
	} else {
		printf("in [General] Name is %s \n", str);
	}

	boolean = g_key_file_get_boolean(config, "General", "RememberPowered", &err);
	if(err) {
		printf("%s\n", err->message);
		g_clear_error(&err);
	} else {
		printf("in [General] RememberPowered is %d\n", (int)boolean);
	}

}
