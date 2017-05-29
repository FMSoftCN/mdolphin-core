/*
** $Id: ProxyMg.h 828 2007-12-28 03:23:38Z jpzhang $
**
** ProxyMg.h: proxy implements header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-15
*/

#ifndef ProxyMg_h
#define ProxyMg_h
#include "minigui.h"
#include "config.h"

#include "CString.h"
#include "mdolphin.h"

namespace WebCore {
class KURL;

struct ProxyItem{
    ProxyItem(char *user, char *pwd, char *host, char *port, int type);
    ProxyItem();
    CString m_user;
    CString m_password;
    CString m_host;
    CString m_port;
    int     m_type;
};

const ProxyItem &proxy(KURL &url);
const ProxyItem *proxy(MDEProxyType type);
void  setProxy(ProxyItem &proxy);
bool proxyEnabled();
void setProxyEnabled(bool status);
}
#endif
