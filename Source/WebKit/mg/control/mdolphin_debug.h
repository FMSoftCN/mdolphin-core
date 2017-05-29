#ifndef _MDOLPHIN_DEBUG_H_
#define _MDOLPHIN_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <minigui/common.h>

#include "mdolphin_binding.h"

// for time interval
JSValueRef md_start_timer (JSContextRef context, JSObjectRef function, 
            JSObjectRef thisObject, size_t argumentCount, 
            const JSValueRef arguments[], JSValueRef* exception);

static JSNativeFunction startTimer = {
        "md_start_timer", md_start_timer, 0
};

JSValueRef md_end_timer (JSContextRef context, JSObjectRef function, 
            JSObjectRef thisObject, size_t argumentCount, 
            const JSValueRef arguments[], JSValueRef* exception);

static JSNativeFunction endTimer = {
        "md_end_timer", md_end_timer, 0
};

JSValueRef md_printf (JSContextRef context, JSObjectRef function, 
            JSObjectRef thisObject, size_t argumentCount, 
            const JSValueRef arguments[], JSValueRef* exception);

static JSNativeFunction myPrintf = {
        "md_printf", md_printf, 0
};

inline void md_debug_nativebinding(void)
{
    mdolphin_define_jsnativefunction(&startTimer);
    mdolphin_define_jsnativefunction(&endTimer);
    mdolphin_define_jsnativefunction(&myPrintf);
}

#ifdef __cplusplus
}  /* end of extern "C" */
#endif

#endif /* _MDOLPHIN_DEBUG_H_ */

