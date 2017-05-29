
#ifndef EventSender_h
#define EventSender_h

typedef const struct OpaqueJSContext* JSContextRef;
typedef struct OpaqueJSValue* JSObjectRef;

JSObjectRef makeEventSender(JSContextRef context, bool isTopFrame);
void replaySavedEvents(long* oleDragAndDropReturnValue = 0);

#endif
