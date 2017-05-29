/*
** $Id: FrameLoaderClientMg.h 1016 2008-03-06 07:04:01Z jpzhang $
**
** FrameLoaderClientMg.h : Frame loader client header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-21
*/

#ifndef WebFrameLoaderClient_H
#define WebFrameLoaderClient_H

#include "FrameLoaderClient.h"

#include "ResourceResponse.h"

class MDWebFrame;

namespace WebCore {

class ResourceRequest;
class FrameMg;

class FrameLoaderClientMg : public FrameLoaderClient
{
public:
    FrameLoaderClientMg();
    virtual ~FrameLoaderClientMg();

    virtual void frameLoaderDestroyed();

    virtual bool hasWebView() const;

    virtual void makeRepresentation(DocumentLoader*);
    virtual void forceLayout();
    virtual void forceLayoutForNonHTML();

    virtual void setCopiesOnScroll();

    virtual void detachedFromParent2();
    virtual void detachedFromParent3();

    virtual void assignIdentifierToInitialRequest(unsigned long identifier, DocumentLoader*, const ResourceRequest&);
    virtual void dispatchWillSendRequest(DocumentLoader*, unsigned long identifier, ResourceRequest&, const ResourceResponse& redirectResponse);
    virtual bool shouldUseCredentialStorage(DocumentLoader*, unsigned long identifier);
    virtual void dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&);
    virtual void dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long identifier, const AuthenticationChallenge&);        
    virtual void dispatchDidReceiveResponse(DocumentLoader*, unsigned long identifier, const ResourceResponse&);
    virtual void dispatchDidReceiveContentLength(DocumentLoader*, unsigned long identifier, int lengthReceived);
    virtual void dispatchDidFinishLoading(DocumentLoader*, unsigned long identifier);
    virtual void dispatchDidFailLoading(DocumentLoader*, unsigned long identifier, const ResourceError&);
    virtual bool dispatchDidLoadResourceFromMemoryCache(DocumentLoader*, const ResourceRequest&, const ResourceResponse&, int length);

    virtual void dispatchDidHandleOnloadEvents();
    virtual void dispatchDidReceiveServerRedirectForProvisionalLoad();
    virtual void dispatchDidCancelClientRedirect();
    virtual void dispatchWillPerformClientRedirect(const KURL&, double, double);
    virtual void dispatchDidChangeLocationWithinPage();
    virtual void dispatchDidPushStateWithinPage();
    virtual void dispatchWillClose();
    virtual void dispatchDidReceiveIcon();
    virtual void dispatchDidStartProvisionalLoad();
    virtual void dispatchDidReceiveTitle(const StringWithDirection&);
    virtual void dispatchDidCommitLoad();
    virtual void dispatchDidFailProvisionalLoad(const ResourceError&);
    virtual void dispatchDidFailLoad(const ResourceError&);
    virtual void dispatchDidFinishDocumentLoad();
    virtual void dispatchDidFinishLoad();
    virtual void dispatchDidFirstLayout();

    virtual Frame* dispatchCreatePage(const NavigationAction&);
    virtual void dispatchShow();

    virtual void dispatchDecidePolicyForResponse(FramePolicyFunction, const ResourceResponse&, const ResourceRequest&);
    virtual void dispatchDecidePolicyForNewWindowAction(FramePolicyFunction, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>, const String& frameName);
    virtual void dispatchDecidePolicyForNavigationAction(FramePolicyFunction, const NavigationAction&, const ResourceRequest&, PassRefPtr<FormState>);
    virtual void cancelPolicyCheck();

    virtual void dispatchUnableToImplementPolicy(const ResourceError&);

    virtual void dispatchWillSubmitForm(FramePolicyFunction,
                                        PassRefPtr<FormState>);

    virtual void dispatchDidLoadMainResource(DocumentLoader*);
    virtual void revertToProvisionalState(DocumentLoader*);
    virtual void setMainDocumentError(DocumentLoader*, const ResourceError&);
    virtual void clearUnarchivingState(DocumentLoader*);

    virtual void setMainFrameDocumentReady(bool);

    virtual void postProgressStartedNotification();
    virtual void postProgressEstimateChangedNotification();
    virtual void postProgressFinishedNotification();
        
    virtual void startDownload(const ResourceRequest&);

    virtual void willChangeTitle(DocumentLoader*);
    virtual void didChangeTitle(DocumentLoader*);

    virtual void committedLoad(DocumentLoader*, const char*, int);
    virtual void finishedLoading(DocumentLoader*);

    virtual void updateGlobalHistoryForStandardLoad(const KURL&);
    virtual void updateGlobalHistoryForReload(const KURL&);
    virtual bool shouldGoToHistoryItem(HistoryItem*) const;
    virtual bool shouldStopLoadingForHistoryItem(WebCore::HistoryItem*) const;

    virtual ResourceError cancelledError(const ResourceRequest&);
    virtual ResourceError blockedError(const ResourceRequest&);
    virtual ResourceError cannotShowURLError(const ResourceRequest&);
    virtual ResourceError interruptForPolicyChangeError(const ResourceRequest&);

    virtual ResourceError cannotShowMIMETypeError(const ResourceResponse&);
    virtual ResourceError fileDoesNotExistError(const ResourceResponse&);

    virtual bool shouldFallBack(const ResourceError&);

    virtual void setDefersLoading(bool);

    virtual bool willUseArchive(ResourceLoader*, const ResourceRequest&, const KURL& originalURL) const;
    virtual bool isArchiveLoadPending(ResourceLoader*) const;
    virtual void cancelPendingArchiveLoad(ResourceLoader*);
    virtual void clearArchivedResources();

    virtual bool canHandleRequest(const ResourceRequest&) const;
    virtual bool canShowMIMEType(const String&) const;
    virtual bool representationExistsForURLScheme(const String&) const;
    virtual String generatedMIMETypeForURLScheme(const String&) const;

    virtual void frameLoadCompleted();
    virtual void saveViewStateToItem(HistoryItem*);
    virtual void restoreViewState();
    virtual void provisionalLoadStarted();
    virtual void didFinishLoad();
    virtual void prepareForDataSourceReplacement();

    virtual PassRefPtr<DocumentLoader> createDocumentLoader(const ResourceRequest&, const SubstituteData&);
    virtual void setTitle(const StringWithDirection& title, const KURL&);

    virtual void didSaveToPageCache();
    virtual void didRestoreFromPageCache();
    virtual String userAgent(const KURL&);
    
    virtual void saveDocumentViewToCachedPage(CachedPage*);
    virtual bool canCachePage() const;
    virtual void download(ResourceHandle*, const ResourceRequest&, const ResourceRequest&, const ResourceResponse&);

    virtual PassRefPtr<Frame> createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement, 
            const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight);
    virtual PassRefPtr<Widget> createPlugin(const IntSize&, HTMLPlugInElement*, const KURL&, const Vector<String>&, const Vector<String>&, const String&, bool loadManually);
    virtual void redirectDataToPlugin(Widget* pluginWidget);

    virtual ObjectContentType objectContentType(const KURL& url, const String& mimeType, bool shouldPreferPlugInsForImages);
    virtual String overrideMediaType() const;

    virtual void windowObjectCleared() const;
        
    virtual void didPerformFirstNavigation() const;
        
    virtual void registerForIconNotification(bool listen = true){}
    //void setFrame (WTF::PassRefPtr<WebCore::Frame> frame){m_webFrame = frame.get();}
    void setWebFrame (MDWebFrame *webFrame);
    MDWebFrame* mdWebFrame() { return m_webFrame; };
    
    virtual void dispatchDidReplaceStateWithinPage();
    virtual void dispatchDidPopStateWithinPage();
    virtual void dispatchDidChangeIcons();
    virtual void dispatchDidFirstVisuallyNonEmptyLayout();
    virtual void dispatchWillSendSubmitEvent(HTMLFormElement*) { }
    virtual void updateGlobalHistory() {}
    virtual void updateGlobalHistoryRedirectLinks() {}
    virtual void dispatchDidAddBackForwardItem(HistoryItem*) const {}
    virtual void dispatchDidRemoveBackForwardItem(HistoryItem*) const {}
    virtual void dispatchDidChangeBackForwardIndex() const {}
    virtual void didDisplayInsecureContent() {}
    virtual void didRunInsecureContent(SecurityOrigin*, const KURL&);
    virtual ResourceError pluginWillHandleLoadError(const ResourceResponse&);
    virtual void savePlatformDataToCachedFrame(WebCore::CachedFrame*) {}
    virtual void transitionToCommittedFromCachedFrame(WebCore::CachedFrame*) {}
    virtual void transitionToCommittedForNewPage();
    virtual void didTransferChildFrameToNewDocument(Page* ) {}
    virtual PassRefPtr<Widget> createJavaAppletWidget(const IntSize&, WebCore::HTMLAppletElement*, const KURL& baseURL, const Vector<String>& paramNames, const Vector<String>& paramValues); 
    virtual void dispatchDidClearWindowObjectInWorld(DOMWrapperWorld*);
    virtual void documentElementAvailable() {}

    virtual bool canShowMIMETypeAsHTML(const String& MIMEType) const {return false;}
    virtual void dispatchDidBecomeFrameset(bool) {}
    virtual void transferLoadingResourceFromPage(unsigned long, DocumentLoader*, const ResourceRequest&, Page*) {}
    virtual PassRefPtr<WebCore::FrameNetworkingContext> createNetworkingContext();

    //FIXME: add by feynman
    virtual void dispatchDidChangeCaret(const IntRect*);
    virtual void  dispatchWillApplyCSSVisitedLink(const KURL &kurl);

private:
    MDWebFrame* m_webFrame;
    bool m_firstData;
    bool m_loadFinished;
};

}

#endif // FrameLoaderClientWin_H
