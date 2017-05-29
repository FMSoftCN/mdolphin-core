/*
** $Id: InspectorClientMg.h 546 2007-10-24 09:26:33Z jpzhang $
**
** InspectorClientMg.h: Inspector Client header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-9-20
*/

#ifndef InspectorClientMg_H
#define InspectorClientMg_H

#include "InspectorClient.h"

#include "InspectorFrontendClientLocal.h"
#include <wtf/Forward.h>

class MDWebView;
class MDWebInspector;
namespace WebCore {

#if ENABLE(INSPECTOR)
class InspectorFrontendClientMg;

class InspectorClientMg : public InspectorClient
{
public:
    InspectorClientMg(MDWebView* );
    virtual ~InspectorClientMg();

    void disconnectFrontendClient() { m_frontendClient = 0; }

    virtual void inspectorDestroyed();

    virtual void openInspectorFrontend(InspectorController*);

    virtual void highlight(Node*);
    virtual void hideHighlight();

    virtual bool sendMessageToFrontend(const WTF::String&);

    void releaseFrontendPage();
	void restoreInspectedWebView();
    const WTF::String& inspectorFilesPath();
private:
    MDWebView* m_inspectedWebView;
    Page* m_frontendPage;
    InspectorFrontendClientMg* m_frontendClient;
    WTF::String m_inspectorFilesPath;
};

class InspectorFrontendClientMg : public WebCore::InspectorFrontendClientLocal {
public:
    InspectorFrontendClientMg(MDWebView* inspectedWebView, MDWebView* inspectorWebView, MDWebInspector* webInspector, 
            Page* inspectorPage, InspectorClientMg* inspectorClient);

    void disconnectInspectorClient() { m_inspectorClient = 0; }

    void destroyInspectorWindow(bool notifyInspectorController);

    virtual WTF::String localizedStringsURL();

    virtual WTF::String hiddenPanels();

    virtual void bringToFront();
    virtual void closeWindow();
    virtual void disconnectFromBackend();

    virtual void attachWindow();
    virtual void detachWindow();

    virtual void setAttachedWindowHeight(unsigned height);

    virtual void inspectedURLChanged(const WTF::String& newURL);

private:
    virtual ~InspectorFrontendClientMg();

    MDWebView* m_inspectorWebView;
    MDWebView* m_inspectedWebView;
    MDWebInspector* m_webInspector;
    InspectorClientMg* m_inspectorClient;
};
#endif

}
#endif // InspectorClientMg_H

