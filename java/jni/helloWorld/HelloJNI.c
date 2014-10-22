#include <stdio.h>
#include "HelloJNI.h"

JNIEXPORT void JNICALL Java_HelloJNI_sayHello
  (JNIEnv* env, jobject object)
{
    printf("Hello World!\n");
    return;
}
