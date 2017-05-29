/*
** $Id: ProxyCurl.cpp 257 2007-07-06 02:43:37Z xwyan $
**
** ProxyCurl.cpp: proxy implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-3-15
*/
#include "minigui.h"
#include "config.h"

#include "KURL.h"
#include "ProxyMg.h"
#include "CString.h"
#include "ResourceHandleManager.h"

namespace WebCore {

struct ProxyControler {
    ProxyControler();
    ProxyItem m_proxys[MD_PROXY_MAX+1];
    bool      m_enable;
};

ProxyControler::ProxyControler()
    : m_enable(false)
{
}

static ProxyControler *proxyCtrl()
{
    static ProxyControler * proxy = new ProxyControler();
    return proxy;
}

ProxyItem::ProxyItem()
    : m_type(-1)
{
}

ProxyItem::ProxyItem(char *user, char *pwd, char *host, char *port, int type)
    : m_user(user)
    , m_password(pwd)
    , m_host(host)
    , m_port(port)
    , m_type(type)
{
}

const ProxyItem &proxy(KURL &url)
{
    if (url.isLocalFile())
        return proxyCtrl()->m_proxys[MD_PROXY_MAX];

    if (url.protocolIs("http") && (!proxyCtrl()->m_proxys[MD_PROXY_HTTP].m_host.isNull()))
        return proxyCtrl()->m_proxys[MD_PROXY_HTTP];

    if (!proxyCtrl()->m_proxys[MD_PROXY_SOCKS5].m_host.isNull())
        return proxyCtrl()->m_proxys[MD_PROXY_SOCKS5];

    if (!proxyCtrl()->m_proxys[MD_PROXY_SOCKS4].m_host.isNull())
        return proxyCtrl()->m_proxys[MD_PROXY_SOCKS4];

    return proxyCtrl()->m_proxys[MD_PROXY_MAX];
}

const ProxyItem *proxy(MDEProxyType type)
{
    return &proxyCtrl()->m_proxys[type];
}

void setProxy(ProxyItem &proxy)
{
    if (proxy.m_type > MD_PROXY_MAX)
        return;

    proxyCtrl()->m_proxys[proxy.m_type] = proxy;
    switch (proxy.m_type) {
        case MD_PROXY_HTTP:
            proxyCtrl()->m_proxys[proxy.m_type].m_type = ResourceHandleManager::HTTP;
            break;
        case MD_PROXY_SOCKS4:
            proxyCtrl()->m_proxys[proxy.m_type].m_type = ResourceHandleManager::Socks4;
            break;
        case MD_PROXY_SOCKS5:
            proxyCtrl()->m_proxys[proxy.m_type].m_type = ResourceHandleManager::Socks5;
            break;
        default:
            LOG_ERROR("Proxy isn't supported!");
            break;
    }
}

bool proxyEnabled()
{
	return proxyCtrl()->m_enable;
}

void setProxyEnabled(bool status)
{
	proxyCtrl()->m_enable = status;
}

}
