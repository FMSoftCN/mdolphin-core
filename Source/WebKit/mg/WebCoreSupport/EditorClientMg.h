/*
** $Id: EditorClientMg.h 596 2007-11-02 07:36:56Z jpzhang $
**
** EditorClientMg.h: Editer Client header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-20
*/

#ifndef EditorClientMg_H
#define EditorClientMg_H

#include "EditorClient.h"

#include "TextCheckerClient.h"
#include "EmptyClients.h"

class MDWebView;
namespace WebCore {

class EditorClientMg : public EditorClient
{
public:
    EditorClientMg(MDWebView *view);
    virtual ~EditorClientMg();
    virtual void pageDestroyed(){delete this;}

    virtual bool shouldDeleteRange(Range*);
    virtual bool shouldShowDeleteInterface(HTMLElement*);
    virtual bool smartInsertDeleteEnabled();
    virtual bool isSelectTrailingWhitespaceEnabled();
    virtual bool isContinuousSpellCheckingEnabled();
    virtual void toggleContinuousSpellChecking();
    virtual bool isGrammarCheckingEnabled();
    virtual void toggleGrammarChecking();
    virtual int spellCheckerDocumentTag();

    virtual bool isEditable();

    virtual bool shouldBeginEditing(Range*);
    virtual bool shouldEndEditing(Range*);
    virtual bool shouldInsertNode(Node*, Range*, EditorInsertAction);
    virtual bool shouldInsertText(const WTF::String&, WebCore::Range*, WebCore::EditorInsertAction) {return true;}
    virtual bool shouldChangeSelectedRange(Range* fromRange, Range* toRange, EAffinity, bool stillSelecting);
    
    virtual bool shouldApplyStyle(CSSStyleDeclaration*,Range*);
    virtual bool shouldMoveRangeAfterDelete(Range*, Range*);
    
    virtual void didBeginEditing();
    virtual void respondToChangedContents();
    virtual void respondToChangedSelection();
    virtual void didEndEditing();
    virtual void didWriteSelectionToPasteboard();
    virtual void didSetSelectionTypesForPasteboard();
    virtual void registerCommandForUndo(PassRefPtr<WebCore::EditCommand>);
    virtual void registerCommandForRedo(PassRefPtr<WebCore::EditCommand>);
    virtual void clearUndoRedoOperations();

    virtual bool canCopyCut(bool defaultValue) const {return defaultValue;}
    virtual bool canPaste(bool defaultValue) const {return defaultValue;}
    virtual bool canUndo() const;
    virtual bool canRedo() const;
    virtual void undo();
    virtual void redo();

    virtual void handleKeyboardEvent(KeyboardEvent*);
    //virtual void handleKeypress(KeyboardEvent*);
    virtual void handleInputMethodKeypress(KeyboardEvent*);
    virtual void handleInputMethodKeydown(WebCore::KeyboardEvent*){}
    
    virtual void textFieldDidBeginEditing(Element*);
    virtual void textFieldDidEndEditing(Element*);
    virtual void textDidChangeInTextField(Element*);
    virtual bool doTextFieldCommandFromEvent(Element*, KeyboardEvent*);
    virtual void textWillBeDeletedInTextField(Element*);
    virtual void textDidChangeInTextArea(Element*);

    virtual void ignoreWordInSpellDocument(const String&);
    virtual void learnWord(const String&);
    virtual void checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength);
    virtual String getAutoCorrectSuggestionForMisspelledWord(const String& misspelledWord); //FIXME
    virtual void checkGrammarOfString(const UChar*, int length, Vector<GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength);

    virtual WebCore::TextCheckerClient* textChecker() { return &m_textCheckerClient; }

    virtual void updateSpellingUIWithGrammarString(const String&, const GrammarDetail& detail);
    virtual void updateSpellingUIWithMisspelledWord(const String&);
    virtual void showSpellingUI(bool show);
    virtual bool spellingUIIsShowing();
    virtual void getGuessesForWord(const WTF::String& word, const WTF::String& context, WTF::Vector<WTF::String>& guesses) {}
    virtual void willSetInputMethodState() {}
    virtual void requestCheckingOfString(WebCore::SpellChecker*, int, const WTF::String&) {}
    virtual void setInputMethodState(bool enabled);
private:
    MDWebView *m_webView;
    WebCore::EmptyTextCheckerClient m_textCheckerClient;
};

}

#endif // EditorClientMg_H
