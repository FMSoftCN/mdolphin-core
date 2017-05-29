/*
** $Id: plugin.h 343 2007-08-8 04:58:57Z dengdd $
**
** plugin.h: flash plugin control registering header file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-8-8
*/

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#if ENABLE(PLUGIN)

#define MDOLPHIN_PLUGIN   ("plugin_ctrl")

namespace WebCore {

BOOL RegisterPluginControl (void);

void UnregisterPluginControl (void);

}

#endif //ENABLE(PLUGIN)

#endif
