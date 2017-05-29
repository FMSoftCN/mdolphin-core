
#ifndef _MINIGUI_H_
#define _MINIGUI_H_


#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

//minigui user message scope: 0x0800~0xEFFF 
//0xEF00~0xEFFF used for mdolphin-core
//0x0800~0xEEFF used for mdolphin application ,such as mdhome,mdtv mdolphin-sample
// gstreamer callback msg
#define MSG_GSTREAMER  0xef00
// LPARAM used for message type
// WPARAM used for message info
#define MSG_MDCORE 0xef10

#endif
