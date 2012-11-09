#include <stdio.h>
#include <locale.h>
#include <libintl.h>

#define _(string) gettext(string)
#define LOCALEDIR "./"
#define PACKAGE "foonly"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	printf(_("Hello, GetText!\n"));

	return 0;
}

