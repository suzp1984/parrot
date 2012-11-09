#include <stdlib.h>
#include <stdio.h>

#ifndef _ED_TYPEDEF_H
#define _ED_TYPEDEF_H

typedef enum _Ret 
{
	RET_OK,
	RET_OOM,
	RET_STOP,
	RET_INVALID_PARAMS,
	RET_FAIL
} Ret;

typedef void     (*DataDestroyFunc)(void* ctx, void* data);
typedef int      (*DataCompareFunc)(void* ctx, void* data);
typedef Ret      (*DataVisitFunc)(void* ctx, void* data);
typedef int       (*DataHashFunc)(void* data);

#define return_if_fail(p) if(!(p)) \
	    {printf("%s:%d Warning: "#p" failed.\n", \
	    		        __func__, __LINE__); return;}
#define return_val_if_fail(p, ret) if(!(p)) \
	    {printf("%s:%d Warning: "#p" failed.\n",\
	    		    __func__, __LINE__); return (ret);}

#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}

#define DECLES_PRIV(priv, thiz) PrivInfo* priv = (PrivInfo*)(thiz->priv)

#ifdef __cplusplus
#define DECLES_BEGIN extern "C" {
#define DECLES_END }
#else
#define DECLES_BEGIN
#define DECLES_END
#endif //__cplusplus

#endif //_ED_TYPEDEF_H
