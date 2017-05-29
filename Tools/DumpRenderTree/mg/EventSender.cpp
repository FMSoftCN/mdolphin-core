#include "minigui.h"
#include <unistd.h>

#include "config.h"
#include "mdolphin.h"
#include "EventSender.h"

#include "DumpRenderTree.h"

#include <JavaScriptCore/JSObjectRef.h>
#include <JavaScriptCore/JSRetainPtr.h>
#include <JavaScriptCore/JSStringRef.h>
#include <wtf/ASCIICType.h>
#include <wtf/Platform.h>

#include <string.h>
#define MSG_DRT_SEND_QUEUED_EVENT (MSG_USER + 100)

static bool down;
static bool dragMode = true;
static bool replayingSavedEvents;
static int timeOffset;
static POINT lastMousePosition;

struct DelayedMessage {
    MSG msg;
    unsigned delay;
};

static DelayedMessage msgQueue[1024];
static unsigned endOfQueue;
static unsigned startOfQueue;

static bool didDragEnter;

static JSValueRef getDragModeCallback(JSContextRef context, JSObjectRef object, 
        JSStringRef propertyName, JSValueRef* exception)
{
    return JSValueMakeBoolean(context, dragMode);
}

static bool setDragModeCallback(JSContextRef context, JSObjectRef object, 
        JSStringRef propertyName, JSValueRef value, JSValueRef* exception)
{
    dragMode = JSValueToBoolean(context, value);
    return true;
}

static JSValueRef getConstantCallback(JSContextRef context, JSObjectRef object, 
        JSStringRef propertyName, JSValueRef* exception)
{
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_KEYDOWN"))
        return JSValueMakeNumber(context, MSG_KEYDOWN);
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_KEYUP"))
        return JSValueMakeNumber(context, MSG_KEYUP);
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_CHAR"))
        return JSValueMakeNumber(context, MSG_CHAR);
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_SYSKEYDOWN"))
        return JSValueMakeNumber(context, MSG_SYSKEYDOWN);
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_SYSKEYUP"))
        return JSValueMakeNumber(context, MSG_SYSKEYUP);
    if (JSStringIsEqualToUTF8CString(propertyName, "MSG_SYSCHAR"))
        return JSValueMakeNumber(context, MSG_SYSCHAR);
    ASSERT_NOT_REACHED();
    return JSValueMakeUndefined(context);
}

static JSValueRef leapForwardCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, 
        const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount > 0) {
        msgQueue[endOfQueue].delay = JSValueToNumber(context, arguments[0], exception);
        ASSERT(!exception || !*exception);
    }

    return JSValueMakeUndefined(context);
}

static DWORD currentEventTime()
{
    return GetTickCount() + timeOffset;
}

static MSG makeMsg(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    MSG result;
    result.hwnd = hwnd;
    result.message = message;
    result.wParam = wParam;
    result.lParam = lParam;
    result.time = currentEventTime();

    return result;
}

static int dispatchMessage(PMSG msg)
{
    ASSERT(msg);
    TranslateMessage(msg);
    return DispatchMessage(msg);
}

static JSValueRef contextClickCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, 
        const JSValueRef arguments[], JSValueRef* exception)
{
    mainFrame->layout();

    down = true;
    MSG msg = makeMsg(webViewWindow, MSG_RBUTTONDOWN, 0, 
            MAKELONG(lastMousePosition.x, lastMousePosition.y));
    dispatchMessage(&msg);
    down = false;
    msg = makeMsg(webViewWindow, MSG_RBUTTONUP, 0, 
            MAKELONG(lastMousePosition.x, lastMousePosition.y));
    dispatchMessage(&msg);
    
    return JSValueMakeUndefined(context);
}

static WPARAM buildModifierFlags(JSContextRef context, const JSValueRef modifiers)
{
    JSObjectRef modifiersArray = JSValueToObject(context, modifiers, 0);
    if (!modifiersArray)
        return 0;

    WPARAM flags = 0;
    int modifiersCount = JSValueToNumber(context, 
        JSObjectGetProperty(context, modifiersArray, JSStringCreateWithUTF8CString("length"), 0), 0);

    for (int i = 0; i < modifiersCount; ++i) {
        JSValueRef value = JSObjectGetPropertyAtIndex(context, modifiersArray, i, 0);
        JSStringRef string = JSValueToStringCopy(context, value, 0);
        if (JSStringIsEqualToUTF8CString(string, "ctrlKey")
            || JSStringIsEqualToUTF8CString(string, "addSelectionKey"))
            flags |= KS_CTRL;
        else if (JSStringIsEqualToUTF8CString(string, "shiftKey")
                 || JSStringIsEqualToUTF8CString(string, "rangeSelectionKey"))
            flags |= KS_SHIFT;
        // No way to specifiy altKey in a MSG.

        JSStringRelease(string);
    }
    return flags;
}

static JSValueRef mouseDownCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, 
        const JSValueRef arguments[], JSValueRef* exception)
{
    mainFrame->layout();

    down = true;
    int mouseType = MSG_LBUTTONDOWN;
    if (argumentCount >= 1) {
        int mouseNumber = JSValueToNumber(context, arguments[0], exception);
        switch (mouseNumber) {
        case 0:
            mouseType = MSG_LBUTTONDOWN;
            break;
        case 1:
            //mouseType = MSG_MBUTTONDOWN;
            break;
        case 2:
            mouseType = MSG_RBUTTONDOWN;
            break;
        case 3:
            // fast/events/mouse-click-events expects the 4th button 
            // has event.button = 1, so send an MSG_BUTTONDOWN
            //mouseType = MSG_MBUTTONDOWN;
            break;
        default:
            mouseType = MSG_LBUTTONDOWN;
            break;
        }
    }

    WPARAM wparam = 0;
    if (argumentCount >= 2)
        wparam |= buildModifierFlags(context, arguments[1]);
        
    MSG msg = makeMsg(webViewWindow, mouseType, 
            wparam, MAKELONG(lastMousePosition.x, lastMousePosition.y));
    if (!msgQueue[endOfQueue].delay)
        dispatchMessage(&msg);
    else {
        // replaySavedEvents has the required logic to make leapForward delays work
        msgQueue[endOfQueue++].msg = msg;
        replaySavedEvents();
    }

    return JSValueMakeUndefined(context);
}

static void doMouseUp(MSG msg, long* oleDragAndDropReturnValue = 0)
{
    mainFrame->layout();
    
    dispatchMessage(&msg);
    down = false;
}

static JSValueRef mouseUpCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, 
        const JSValueRef arguments[], JSValueRef* exception)
{
    int mouseType = MSG_LBUTTONUP;
    if (argumentCount >= 1) {
        int mouseNumber = JSValueToNumber(context, arguments[0], exception);
        switch (mouseNumber) {
        case 0:
            mouseType = MSG_LBUTTONUP;
            break;
        case 1:
            //mouseType = MSG_MBUTTONUP;
            break;
        case 2:
            mouseType = MSG_RBUTTONUP;
            break;
        case 3:
            // fast/events/mouse-click-events expects the 4th button 
            // has event.button = 1, so send an MSG_MBUTTONUP
            //mouseType = MSG_MBUTTONUP;
            break;
        default:
            mouseType = MSG_LBUTTONUP;
            break;
        }
    }

    WPARAM wparam = 0;
    if (argumentCount >= 2)
        wparam |= buildModifierFlags(context, arguments[1]);

    MSG msg = makeMsg(webViewWindow, mouseType, 
            wparam, MAKELONG(lastMousePosition.x, lastMousePosition.y));

    if ((dragMode && !replayingSavedEvents) || msgQueue[endOfQueue].delay) {
        msgQueue[endOfQueue++].msg = msg;
        replaySavedEvents();
    } else
        doMouseUp(msg);

    return JSValueMakeUndefined(context);
}

static void doMouseMove(MSG msg)
{
    mainFrame->layout();
    dispatchMessage(&msg);
    //TODO:current don't support dragging.
}

static JSValueRef mouseMoveToCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, 
        const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 2)
        return JSValueMakeUndefined(context);

    lastMousePosition.x = (int)JSValueToNumber(context, arguments[0], exception);
    ASSERT(!exception || !*exception);
    lastMousePosition.y = (int)JSValueToNumber(context, arguments[1], exception);
    ASSERT(!exception || !*exception);

    MSG msg = makeMsg(webViewWindow, MSG_MOUSEMOVE, down ? KS_LEFTBUTTON : 0, 
            MAKELONG(lastMousePosition.x, lastMousePosition.y));

    if (dragMode && down && !replayingSavedEvents) {
        msgQueue[endOfQueue++].msg = msg;
        return JSValueMakeUndefined(context);
    }

    doMouseMove(msg);

    return JSValueMakeUndefined(context);
}

void replaySavedEvents(long* oleDragAndDropReturnValue)
{
    IMDWebView* view = mainFrame->webView();
    if (!view)
        return;

    replayingSavedEvents = true;
  
    MSG msg;

    while (startOfQueue < endOfQueue && !msgQueue[startOfQueue].delay) {
        msg = msgQueue[startOfQueue++].msg;
        switch (msg.message) {
            case MSG_LBUTTONUP:
            case MSG_RBUTTONUP:
            //case MSG_MBUTTONUP:
                doMouseUp(msg, oleDragAndDropReturnValue);
                break;
            case MSG_MOUSEMOVE:
                doMouseMove(msg);
                break;
            case MSG_LBUTTONDOWN:
            case MSG_RBUTTONDOWN:
            //case MSG_MBUTTONDOWN:
                dispatchMessage(&msg);
                break;
            default:
                // Not reached
                break;
        }
    }

    int numQueuedMessages = endOfQueue - startOfQueue;
    if (!numQueuedMessages) {
        startOfQueue = 0;
        endOfQueue = 0;
        replayingSavedEvents = false;
        ASSERT(!down);
        return;
    }

    if (msgQueue[startOfQueue].delay) {
        usleep(msgQueue[startOfQueue].delay);
        msgQueue[startOfQueue].delay = 0;
    }

    PostMessage(webViewWindow, MSG_DRT_SEND_QUEUED_EVENT, 0, 0);
    while (GetMessage(&msg, webViewWindow)) {
        if (msg.message == MSG_MOUSEMOVEIN && !(msg.wParam))
            continue;
        if (msg.message != MSG_DRT_SEND_QUEUED_EVENT) {
            dispatchMessage(&msg);
            continue;
        }
        msg = msgQueue[startOfQueue++].msg;
        switch (msg.message) {
            case MSG_LBUTTONUP:
            case MSG_RBUTTONUP:
            //case MSG_MBUTTONUP:
                doMouseUp(msg, oleDragAndDropReturnValue);
                break;
            case MSG_MOUSEMOVE:
                doMouseMove(msg);
                break;
            case MSG_LBUTTONDOWN:
            case MSG_RBUTTONDOWN:
            //case MSG_MBUTTONDOWN:
                dispatchMessage(&msg);
                break;
            default:
                // Not reached
                break;
        }
        if (startOfQueue >= endOfQueue)
            break;
        usleep(msgQueue[startOfQueue].delay);
        msgQueue[startOfQueue].delay = 0;
        PostMessage(webViewWindow, MSG_DRT_SEND_QUEUED_EVENT, 0, 0);
    }
    startOfQueue = 0;
    endOfQueue = 0;

    replayingSavedEvents = false;
}

static int mg_unicode_to_scancode(int charCode)
{
    static char ascii_maps[]={
        27,//ESCAP
        '1','2','3','4','5','6','7','8','9','0','-','=', 8 /* backspace */,
        9 /* TAB */, 'Q', 'W','E','R','T', 'Y','U','I','O','P','[',']','\n',
        0, /* LEFT_CONTRL */ 
        'A','S','D','F','G','H','J','K','L',';','\'', 
        0 /* SCANCODE_GRAVE */, 0 /* LEFT_SHIFT */, '\\',
        'Z','X','C','V','B','N','M',',','.','/',
        0, 0, 0, /*RIGHT SHIFT, KEYPAD MULTIPLY, LEFT ALT  */
        ' '
    };

    unsigned int i;
    for (i = 0; i < sizeof(ascii_maps)/sizeof(char); i++)
        if (ascii_maps[i] == charCode)
            return i + 1;

    return 0;
}

static JSValueRef keyDownCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 1)
        return JSValueMakeUndefined(context);

    static const JSStringRef lengthProperty = JSStringCreateWithUTF8CString("length");
    mainFrame->layout();

    JSStringRef character = JSValueToStringCopy(context, arguments[0], exception);
    ASSERT(!*exception);

    int scanCode;
    int charCode = 0;
    int state = 0;
    //handle modifier keys.
    if (argumentCount > 1) {
        JSObjectRef modifiersArray = JSValueToObject(context, arguments[1], exception);

        if (modifiersArray) {
            for (int i = 0; i < JSValueToNumber(context, JSObjectGetProperty(context, modifiersArray, lengthProperty, 0), 0); ++i) {
                JSValueRef value = JSObjectGetPropertyAtIndex(context, modifiersArray, i, 0);
                JSStringRef string = JSValueToStringCopy(context, value, 0);
                if (JSStringIsEqualToUTF8CString(string, "ctrlKey"))
                    state |= KS_CTRL;
                else if (JSStringIsEqualToUTF8CString(string, "shiftKey"))
                    state |= KS_SHIFT;
                else if (JSStringIsEqualToUTF8CString(string, "altKey"))
                    state |= KS_ALT;

                JSStringRelease(string);
            }
        }
    }

    if (JSStringIsEqualToUTF8CString(character, "leftArrow")) {
        scanCode = SCANCODE_CURSORBLOCKLEFT;
    } else if (JSStringIsEqualToUTF8CString(character, "rightArrow")) {
        scanCode = SCANCODE_CURSORBLOCKRIGHT;
    } else if (JSStringIsEqualToUTF8CString(character, "upArrow")) {
        scanCode = SCANCODE_CURSORBLOCKUP;
    } else if (JSStringIsEqualToUTF8CString(character, "downArrow")) {
        scanCode = SCANCODE_CURSORBLOCKDOWN;
    } else if (JSStringIsEqualToUTF8CString(character, "pageUp"))
        scanCode = SCANCODE_PAGEUP;
    else if (JSStringIsEqualToUTF8CString(character, "pageDown"))
        scanCode = SCANCODE_PAGEDOWN;
    else if (JSStringIsEqualToUTF8CString(character, "home"))
        scanCode = SCANCODE_HOME;
    else if (JSStringIsEqualToUTF8CString(character, "end"))
        scanCode = SCANCODE_END;
    else if (JSStringIsEqualToUTF8CString(character, "delete"))
        scanCode = SCANCODE_REMOVE;
    else if (JSStringIsEqualToUTF8CString(character, "F1"))
        scanCode = SCANCODE_F1;
    else if (JSStringIsEqualToUTF8CString(character, "F2"))
        scanCode = SCANCODE_F2;
    else if (JSStringIsEqualToUTF8CString(character, "F3"))
        scanCode = SCANCODE_F3;
    else if (JSStringIsEqualToUTF8CString(character, "F4"))
        scanCode = SCANCODE_F4;
    else if (JSStringIsEqualToUTF8CString(character, "F5"))
        scanCode = SCANCODE_F5;
    else if (JSStringIsEqualToUTF8CString(character, "F6"))
        scanCode = SCANCODE_F6;
    else if (JSStringIsEqualToUTF8CString(character, "F7"))
        scanCode = SCANCODE_F7;
    else if (JSStringIsEqualToUTF8CString(character, "F8"))
        scanCode = SCANCODE_F8;
    else if (JSStringIsEqualToUTF8CString(character, "F9"))
        scanCode = SCANCODE_F9;
    else if (JSStringIsEqualToUTF8CString(character, "F10"))
        scanCode = SCANCODE_F10;
    else if (JSStringIsEqualToUTF8CString(character, "F11"))
        scanCode = SCANCODE_F11;
    else if (JSStringIsEqualToUTF8CString(character, "F12"))
        scanCode = SCANCODE_F12;
    else {
        charCode = JSStringGetCharactersPtr(character)[0];
        if (charCode == '\n' || charCode == '\r')
            scanCode = SCANCODE_ENTER;
        else if (charCode == '\t')
            scanCode = SCANCODE_TAB;
        else if (charCode == '\x8')
            scanCode = SCANCODE_BACKSPACE;
        else {
            scanCode = mg_unicode_to_scancode(charCode);
            if (WTF::isASCIIUpper(charCode))
                state |= KS_SHIFT;
        }
    }
    JSStringRelease(character);

    MSG msg = makeMsg(webViewWindow, 
            GetKeyStatus(SCANCODE_MENU) ? MSG_SYSKEYDOWN : MSG_KEYDOWN, 
            scanCode, state);
    if (scanCode != 255)
        dispatchMessage(&msg);
    else {
        // For characters that do not exist in the active keyboard layout,
        // Translate will not work, so we post an MSG_CHAR event ourselves.
        PostMessage(webViewWindow, MSG_CHAR, charCode, 0);
    }

    // Tests expect that all messages are processed by the time keyDown() returns.
    if (PeekMessage(&msg, webViewWindow, MSG_CHAR, MSG_CHAR, PM_REMOVE) 
            || PeekMessage(&msg, webViewWindow, MSG_SYSCHAR, MSG_SYSCHAR, PM_REMOVE))
        DispatchMessage(&msg);

    MSG msgUp = makeMsg(webViewWindow, 
            GetKeyStatus(SCANCODE_MENU) ? MSG_SYSKEYUP : MSG_KEYUP, 
            scanCode, state);
    DispatchMessage(&msgUp);

    return JSValueMakeUndefined(context);
}

// eventSender.dispatchMessage(message, wParam, lParam, time = currentEventTime(), x = lastMousePosition.x, y = lastMousePosition.y)
static JSValueRef dispatchMessageCallback(JSContextRef context, JSObjectRef function, 
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 3)
        return JSValueMakeUndefined(context);

    mainFrame->layout();
    
    MSG msg;
    msg.hwnd = webViewWindow;
    msg.message = JSValueToNumber(context, arguments[0], exception);
    ASSERT(!*exception);
    msg.wParam = JSValueToNumber(context, arguments[1], exception);
    ASSERT(!*exception);
    msg.lParam = static_cast<DWORD>(JSValueToNumber(context, arguments[2], exception));
    ASSERT(!*exception);
    if (argumentCount >= 4) {
        msg.time = JSValueToNumber(context, arguments[3], exception);
        ASSERT(!*exception);
    }
    if (!msg.time)
        msg.time = currentEventTime();
    if (argumentCount >= 6) {
        //set cursor pos
#if 0
        msg.pt.x = JSValueToNumber(context, arguments[4], exception);
        ASSERT(!*exception);
        msg.pt.y = JSValueToNumber(context, arguments[5], exception);
        ASSERT(!*exception);
#endif
    }

    DispatchMessage(&msg);

    return JSValueMakeUndefined(context);
}

static const float zoomMultiplierRatio = 1.2f;
static void zoomIn(bool fullContentsZoom)
{
    IMDWebView* view = mainFrame->webView();
    if (!view)
        return;

    float currentZoom = 1.0;
    if (fullContentsZoom) {
        view->setPageSizeMultiplier(currentZoom * zoomMultiplierRatio);
    }
    else {
        //for text
        view->setTextSizeMultiplier(currentZoom * zoomMultiplierRatio);
    }
}

static void zoomOut(bool fullContentsZoom)
{
    IMDWebView* view = mainFrame->webView();
    if (!view)
        return;

    float currentZoom = 1.0;
    if (fullContentsZoom) {
        view->setPageSizeMultiplier(currentZoom * zoomMultiplierRatio);
    }
    else {
        //for text
        view->setTextSizeMultiplier(currentZoom * zoomMultiplierRatio);
    }
}

static JSValueRef textZoomInCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    zoomIn(false);
    return JSValueMakeUndefined(context);
}

static JSValueRef textZoomOutCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    zoomOut(false);
    return JSValueMakeUndefined(context);
}

static JSValueRef zoomPageInCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    zoomIn(true);
    return JSValueMakeUndefined(context);
}

static JSValueRef zoomPageOutCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    zoomOut(true);
    return JSValueMakeUndefined(context);
}

static JSStaticFunction staticFunctions[] = {
    { "contextClick", contextClickCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "mouseDown", mouseDownCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "mouseUp", mouseUpCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "mouseMoveTo", mouseMoveToCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "leapForward", leapForwardCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "keyDown", keyDownCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "dispatchMessage", dispatchMessageCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "textZoomIn", textZoomInCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "textZoomOut", textZoomOutCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "zoomPageIn", zoomPageInCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { "zoomPageOut", zoomPageOutCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { 0, 0, 0 }
};

static JSStaticValue staticValues[] = {
    { "dragMode", getDragModeCallback, setDragModeCallback, kJSPropertyAttributeNone },
    { "MSG_KEYDOWN", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { "MSG_KEYUP", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { "MSG_CHAR", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { "MSG_SYSKEYDOWN", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { "MSG_SYSKEYUP", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { "MSG_SYSCHAR", getConstantCallback, 0, kJSPropertyAttributeReadOnly | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

static JSClassRef getClass(JSContextRef context)
{
    static JSClassRef eventSenderClass = 0;

    if (!eventSenderClass) {
        JSClassDefinition classDefinition = {
                0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        classDefinition.staticFunctions = staticFunctions;
        classDefinition.staticValues = staticValues;

        eventSenderClass = JSClassCreate(&classDefinition);
    }

    return eventSenderClass;
}

JSObjectRef makeEventSender(JSContextRef context, bool isTopFrame)
{
    if (isTopFrame) {
        down = false;
        dragMode = true;
        replayingSavedEvents = false;
        timeOffset = 0;
        lastMousePosition.x = 0;
        lastMousePosition.y = 0;

        endOfQueue = 0;
        startOfQueue = 0;

        didDragEnter = false;
    }
    return JSObjectMake(context, getClass(context), 0);
}
