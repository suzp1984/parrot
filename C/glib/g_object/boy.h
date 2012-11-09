#ifndef _BOY_H
#define _BOY_H

#include <glib-object.h>
#include <glib.h>
#define BOY_TYPE (boy_get_type())
#define BOY(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),BOY_TYPE,Boy))

typedef struct _Boy Boy;
typedef struct _BoyClass BoyClass;

struct _Boy {
	GObject parent;
	gint age;
	gchar *name;
	void (*cry)(void);
};

struct _BoyClass {
	GObjectClass parent_class;
	void (*boy_born)(void);
};

GType boy_get_type(void);
Boy* boy_new(void);
int boy_get_age(Boy* boy);
void boy_set_age(Boy* boy, int age);
char* boy_get_name(Boy* boy);
void boy_set_name(Boy* boy, char* name);

Boy* boy_new_with_name(gchar* name);
Boy* boy_new_with_age(gint age);
Boy* boy_new_with_name_and_age(gchar* name, gint age);
void boy_info(Boy* boy);

#endif // _BOY_H
