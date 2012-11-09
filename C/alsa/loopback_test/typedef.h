#ifndef TYPE_DEF_H
#define TYPE_DEF_H

#include <stdlib.h>
#include <stdio.h>

typedef enum _Ret
{
	RET_OK,
	RET_OOM,
	RET_STOP,
	RET_INVALID_PARAMS,
	RET_FAIL
} Ret;

#define DECLES_PRIV(p, ob) PrivInfo* p = (PrivInfo*) ob->priv
#define LOGE(args...) \
{ \
	printf(args); \
	printf("\n"); \
}

#define return_if_fail(p) if(!(p)) \
			{LOGE("%s:%d Warning: "#p" failed.\n", \
				__func__, __LINE__); return;}
#define return_val_if_fail(p, ret) if(!(p)) \
			{LOGE("%s:%d Warning: "#p" failed.\n",\
				__func__, __LINE__); return (ret);}

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

#ifdef __cplusplus
#define DECLES_BEGIN extern "C" {
#define DECLES_END }
#else
#define DECLES_BEGIN
#define DECLES_END
#endif //__cplusplus


#endif // TYPE_DEF_H 
