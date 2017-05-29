/*
** $Id: EditorClientMg.cpp 1562 2008-11-18 03:52:03Z yangyan $
**
** EditorClientMg.h: Editer Client implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-10-31
*/

#include "minigui.h"
#include "mdolphin.h"

#include "config.h"
#include <wtf/HashMap.h>
#include "Frame.h"
#include "Page.h"
#include "FocusController.h"
#include "EditorClientMg.h"
#include "Document.h"
#include "Editor.h"
#include "KeyboardEvent.h"
#include "PlatformKeyboardEvent.h"
#include "KeyboardCodes.h"
#include "ScanCodeMap.h"
#include "CString.h"
#include "MDWebView.h"
#include "NotImplemented.h"
#include "MDolphinDelegates.h"
#include "RenderView.h"
#include "HTMLInputElement.h"
#include "HTMLTextAreaElement.h"
#include "HTMLSelectElement.h"
#if ENABLE(WML)
#include "WMLNames.h"
#include "WMLInputElement.h"
#endif
using namespace Control;

namespace WebCore
{

static HashMap<unsigned int, const char*>* commandsMap = NULL;

EditorClientMg::EditorClientMg(MDWebView *view)
    :m_webView(view)
{
    if (!commandsMap) {
        commandsMap = new HashMap<unsigned int, const char*>;
        for (size_t i = 0; i < TABLESIZE(keyEntries); i++)
            commandsMap->set(keyEntries[i].modifiers << 16 | keyEntries[i].virtualKey, keyEntries[i].name);
    }
}

EditorClientMg::~EditorClientMg()
{
}

bool EditorClientMg::isEditable()
{
    return false;
}

bool EditorClientMg::shouldChangeSelectedRange(Range* fromRange, Range* toRange, EAffinity, bool stillSelecting)
{
    return true;
}

bool EditorClientMg::shouldDeleteRange(Range*)
{
    return true;
}

bool EditorClientMg::shouldBeginEditing(Range*)
{
    return true;
}

bool EditorClientMg::shouldEndEditing(Range*)
{
    return true;
}
#if 0
bool EditorClientMg::shouldInsertText(String, Range*, EditorInsertAction)
{
    return true;
}
#endif

bool EditorClientMg::shouldApplyStyle(CSSStyleDeclaration*,Range*)
{
    return true;
}

bool EditorClientMg::shouldMoveRangeAfterDelete(Range*, Range*)
{
    return true;
}

bool EditorClientMg::shouldInsertNode(Node*, Range*, EditorInsertAction)
{
    return true;
}

//please refrence http://bugzilla.rd.minigui.com/show_bug.cgi?id=5499
void EditorClientMg::setInputMethodState(bool enabled)
{
    // judge if need ime
    IMDWebUIDelegate *uiDelegate = m_webView->uiDelegate();
    ASSERT(uiDelegate);

    // Return if not has ime
    if (!uiDelegate->hasCustomerIME())
        return;

    uiDelegate->setCustomerIME(m_webView, enabled);
}

static const char * translate_key_event (const PlatformKeyboardEvent* keyEvent)
{
    unsigned modifiers = 0;
    if (keyEvent->shiftKey())
        modifiers |= ShiftKey;
    if (keyEvent->altKey())
        modifiers |= AltKey;
    if (keyEvent->ctrlKey())
        modifiers |= CtrlKey;

    if (keyEvent->windowsVirtualKeyCode() == 0 && modifiers == 0)
        return NULL;

    return commandsMap->get(modifiers << 16 | keyEvent->windowsVirtualKeyCode());
}

void EditorClientMg::handleInputMethodKeypress(KeyboardEvent* event)
{
    handleKeyboardEvent(event);
}

void EditorClientMg::handleKeyboardEvent(KeyboardEvent* event)
{
    ASSERT(core(m_webView));
    Frame* frame = core(m_webView)->focusController()->focusedOrMainFrame();
    if (!frame || !frame->document()->focusedNode())
        return;

    const PlatformKeyboardEvent* kevent = event->keyEvent();
    if (kevent->type() == PlatformKeyboardEvent::KeyUp)
        return;

    //FIXME:lijiangwei
    //Node* start = frame->selection()->start().node();
    Node* start = event->target()->toNode();
    if (!start)
        return;

    String textcommand(translate_key_event(kevent));
    Editor::Command command = frame->editor()->command(textcommand);
    if (kevent->type() == PlatformKeyboardEvent::RawKeyDown) {
        if (!command.isTextInsertion() && command.execute(event)) {
            event->setDefaultHandled();
        } else if (kevent->windowsVirtualKeyCode() == '\r' && command.execute(event)) {
            event->setDefaultHandled();
        }
        return;
    }

    if (command.execute(event)) {
        event->setDefaultHandled();
        return;
    }

    if (event->charCode() < ' ')
        return;

    if (frame->editor()->insertText(event->keyEvent()->text(), event))
        event->setDefaultHandled();

    return;
}

void EditorClientMg::respondToChangedSelection()
{
    ASSERT(core(m_webView));
    Frame* frame = core(m_webView)->focusController()->focusedOrMainFrame();
    if (!frame || !frame->editor()->hasComposition())
        return;

    if (frame->editor()->ignoreCompositionSelectionChange())
        return;

    unsigned start;
    unsigned end;
    if (!frame->editor()->getCompositionSelection(start, end))
        frame->editor()->confirmCompositionWithoutDisturbingSelection();
}

String EditorClientMg::getAutoCorrectSuggestionForMisspelledWord(const String& inputWord)
{
    // This method can be implemented using customized algorithms for the particular browser.
    // Currently, it computes an empty string.
    return String();
}

bool EditorClientMg::isSelectTrailingWhitespaceEnabled()
{
    notImplemented();
    return false;
}

//not implements
bool EditorClientMg::shouldShowDeleteInterface(HTMLElement*)
{
    notImplemented();
    return false;
}
bool EditorClientMg::smartInsertDeleteEnabled()
{
    notImplemented();
    return false;
}
bool EditorClientMg::isContinuousSpellCheckingEnabled()
{
    notImplemented();
    return false;
}
void EditorClientMg::toggleContinuousSpellChecking()
{
    notImplemented();
}
bool EditorClientMg::isGrammarCheckingEnabled()
{
    notImplemented();
    return false;
}
void EditorClientMg::toggleGrammarChecking()
{
    notImplemented();
}
int EditorClientMg::spellCheckerDocumentTag()
{
    notImplemented();
    return 0;
}
void EditorClientMg::didBeginEditing()
{
    notImplemented();
}
void EditorClientMg::respondToChangedContents()
{
    notImplemented();
}
void EditorClientMg::didEndEditing()
{
    notImplemented();
}
void EditorClientMg::didWriteSelectionToPasteboard()
{
    notImplemented();
}
void EditorClientMg::didSetSelectionTypesForPasteboard()
{
    notImplemented();
}
void EditorClientMg::registerCommandForUndo(PassRefPtr<WebCore::EditCommand>)
{
    notImplemented();
}
void EditorClientMg::registerCommandForRedo(PassRefPtr<WebCore::EditCommand>)
{
    notImplemented();
}
void EditorClientMg::clearUndoRedoOperations()
{
    notImplemented();
}
bool EditorClientMg::canUndo() const
{
    notImplemented();
    return false;
}
bool EditorClientMg::canRedo() const
{
    notImplemented();
    return false;
}
void EditorClientMg::undo()
{
    notImplemented();
}
void EditorClientMg::redo()
{
    notImplemented();
}
void EditorClientMg::textFieldDidBeginEditing(Element*)
{
    notImplemented();
}
void EditorClientMg::textFieldDidEndEditing(Element*)
{
    notImplemented();
}
void EditorClientMg::textDidChangeInTextField(Element*)
{
    notImplemented();
}
bool EditorClientMg::doTextFieldCommandFromEvent(Element*, KeyboardEvent*)
{
    notImplemented();
    return false;
}
void EditorClientMg::textWillBeDeletedInTextField(Element*)
{
    notImplemented();
}
void EditorClientMg::textDidChangeInTextArea(Element*)
{
    notImplemented();
}
void EditorClientMg::ignoreWordInSpellDocument(const String&)
{
    notImplemented();
}
void EditorClientMg::learnWord(const String&)
{
    notImplemented();
}
void EditorClientMg::checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength)
{
    notImplemented();
}
void EditorClientMg::checkGrammarOfString(const UChar*, int length, Vector<GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength)
{
    notImplemented();
}
void EditorClientMg::updateSpellingUIWithGrammarString(const String&, const GrammarDetail& detail)
{
    notImplemented();
}
void EditorClientMg::updateSpellingUIWithMisspelledWord(const String&)
{
    notImplemented();
}
void EditorClientMg::showSpellingUI(bool show)
{
    notImplemented();
}
bool EditorClientMg::spellingUIIsShowing()
{
    notImplemented();
    return false;
}

}
