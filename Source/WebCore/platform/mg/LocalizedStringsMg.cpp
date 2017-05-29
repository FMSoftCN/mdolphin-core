/*
 ** $Id: LocalizedStringsMg.cpp 618 2011-05-03 07:37:44Z lijiangwei $
 **
 ** LocalizedStringsMg.cpp: The platform implementation for LocalizedStrings.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "config.h"
#include "LocalizedStrings.h"

#include "NotImplemented.h"
#include "PlatformString.h"

namespace WebCore {

String fileButtonChooseFileLabel()
{
    return String("Choose File");
}

String fileButtonNoFileSelectedLabel()
{
    return String("No file selected");
}

String contextMenuItemTagOpenLinkInNewWindow()
{
    notImplemented();
    return String("Open Link in New Window");
}

String contextMenuItemTagDownloadLinkToDisk()
{
    notImplemented();
    return String("Download Linked File");
}

String contextMenuItemTagCopyLinkToClipboard()
{
    notImplemented();
    return String("Copy Link Loc_ation");
}

String contextMenuItemTagOpenImageInNewWindow()
{
    notImplemented();
    return String("Open _Image in New Window");
}

String contextMenuItemTagDownloadImageToDisk()
{
    notImplemented();
    return String("Sa_ve Image As");
}

String contextMenuItemTagCopyImageToClipboard()
{
    notImplemented();
    return String("Cop_y Image");
}

String contextMenuItemTagOpenFrameInNewWindow()
{
    notImplemented();
    return String("Open _Frame in New Window");
}

String contextMenuItemTagCopy()
{
    notImplemented();
    return String();
}

String contextMenuItemTagDelete()
{
    notImplemented();
    return String();
}


String contextMenuItemTagSelectAll()
{
    notImplemented();
    return String();
}

String contextMenuItemTagUnicode()
{
    notImplemented();
    return String("_Insert Unicode Control Character");
}

String contextMenuItemTagInputMethods()
{
    notImplemented();
    return String("Input _Methods");
}

String contextMenuItemTagGoBack()
{
    notImplemented();
    return String();
}

String contextMenuItemTagGoForward()
{
    notImplemented();
    return String();
}

String contextMenuItemTagStop()
{
    notImplemented();
    return String();
}

String contextMenuItemTagReload()
{
    notImplemented();
    return String();
}

String contextMenuItemTagCut()
{
    notImplemented();
    return String();
}

String contextMenuItemTagPaste()
{
    notImplemented();
    return String();
}

String contextMenuItemTagNoGuessesFound()
{
    notImplemented();
    return String("No Guesses Found");
}

String contextMenuItemTagIgnoreSpelling()
{
    notImplemented();
    return String("_Ignore Spelling");
}

String contextMenuItemTagLearnSpelling()
{
    notImplemented();
    return String("_Learn Spelling");
}

String contextMenuItemTagSearchWeb()
{
    notImplemented();
    return String("_Search the Web");
}

String contextMenuItemTagLookUpInDictionary()
{
    notImplemented();
    return String("_Look Up in Dictionary");
}

String contextMenuItemTagOpenLink()
{
    notImplemented();
    return String("_Open Link");
}

String contextMenuItemTagIgnoreGrammar()
{
    notImplemented();
    return String("Ignore _Grammar");
}

String contextMenuItemTagSpellingMenu()
{
    notImplemented();
    return String("Spelling and _Grammar");
}

String contextMenuItemTagShowSpellingPanel(bool show)
{
    notImplemented();
    return String(show ? "_Show Spelling and Grammar" : "_Hide Spelling and Grammar");
}

String contextMenuItemTagCheckSpelling()
{
    notImplemented();
    return String("_Check Document Now");
}

String contextMenuItemTagCheckSpellingWhileTyping()
{
    notImplemented();
    return String("Check Spelling While _Typing");
}

String contextMenuItemTagCheckGrammarWithSpelling()
{
    notImplemented();
    return String("Check _Grammar With Spelling");
}

String contextMenuItemTagFontMenu()
{
    notImplemented();
    return String("_Font");
}

String contextMenuItemTagBold()
{
    notImplemented();
    return String();
}

String contextMenuItemTagItalic()
{
    notImplemented();
    return String();
}

String contextMenuItemTagUnderline()
{
    notImplemented();
    return String();
}

String contextMenuItemTagOutline()
{
    notImplemented();
    return String("_Outline");
}

String contextMenuItemTagInspectElement()
{
    return String("Inspect Element");
}

String searchMenuNoRecentSearchesText()
{
    return String("No recent searches");
}

String searchMenuRecentSearchesText()
{
    return String("Recent searches");
}

String searchMenuClearRecentSearchesText()
{
    return String("_Clear recent searches");
}

String AXDefinitionListTermText()
{
    notImplemented();
    return String("term");
}

String AXDefinitionListDefinitionText()
{
    notImplemented();
    return String("definition");
}

String AXButtonActionVerb()
{
    notImplemented();
    return String("press");
}

String AXRadioButtonActionVerb()
{
    notImplemented();
    return String("select");
}

String AXTextFieldActionVerb()
{
    notImplemented();
    return String("activate");
}

String AXCheckedCheckBoxActionVerb()
{
    notImplemented();
    return String("uncheck");
}

String AXUncheckedCheckBoxActionVerb()
{
    notImplemented();
    return String("check");
}

String AXLinkActionVerb()
{
    notImplemented();
    return String("jump");
}

String AXMenuListPopupActionVerb()
{
    notImplemented();
    return String();
}

String AXMenuListActionVerb()
{
    notImplemented();
    return String();
}

String missingPluginText()
{
    notImplemented();
    return String("Missing Plug-in");
}

String crashedPluginText()
{
    notImplemented();
    return String("Plug-in Failure");
}

String multipleFileUploadText(unsigned numberOfFiles)
{
    return String::number(numberOfFiles) + String(" files");
}

String unknownFileSizeText()
{
    return String("Unknown");
}


String contextMenuItemTagWritingDirectionMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagTextDirectionMenu()
{
    notImplemented();
    return String();
}

String contextMenuItemTagDefaultDirection()
{
    notImplemented();
    return String();
}

String contextMenuItemTagLeftToRight()
{
    notImplemented();
    return String();
}

String contextMenuItemTagRightToLeft()
{
    notImplemented();
    return String();
}

String imageTitle(const String& filename, const IntSize& size)
{
    notImplemented();
    return String();
}

#if ENABLE(VIDEO)
String mediaElementLoadingStateText()
{
    notImplemented();
    return String("Loading...");
}

String mediaElementLiveBroadcastStateText()
{
    notImplemented();
    return String("Live Broadcast");
}

String localizedMediaControlElementString(const String& controlName)
{
    notImplemented();

    if (controlName == "AudioElement")
        return String("audio element controller");
    if (controlName == "VideoElement")
        return String("video element controller");
    if (controlName == "MuteButton")
        return String("mute");
    if (controlName == "UnMuteButton")
        return String("unmute");
    if (controlName == "PlayButton")
        return String("play");
    if (controlName == "PauseButton")
        return String("pause");
    if (controlName == "Slider")
        return String("movie time");
    if (controlName == "SliderThumb")
        return String("timeline slider thumb");
    if (controlName == "RewindButton")
        return String("back 30 seconds");
    if (controlName == "ReturnToRealtimeButton")
        return String("return to realtime");
    if (controlName == "CurrentTimeDisplay")
        return String("elapsed time");
    if (controlName == "TimeRemainingDisplay")
        return String("remaining time");
    if (controlName == "StatusDisplay")
        return String("status");
    if (controlName == "FullscreenButton")
        return String("fullscreen");
    if (controlName == "SeekForwardButton")
        return String("fast forward");
    if (controlName == "SeekBackButton")
        return String("fast reverse");
    if (controlName == "ShowClosedCaptionsButton")
        return String("show closed captions");
    if (controlName == "HideClosedCaptionsButton")
        return String("hide closed captions");

    ASSERT_NOT_REACHED();
    return String();
}

String localizedMediaControlElementHelpText(const String& controlName)
{
    notImplemented();

    if (controlName == "AudioElement")
        return String("audio element playback controls and status display");
    if (controlName == "VideoElement")
        return String("video element playback controls and status display");
    if (controlName == "MuteButton")
        return String("mute audio tracks");
    if (controlName == "UnMuteButton")
        return String("unmute audio tracks");
    if (controlName == "PlayButton")
        return String("begin playback");
    if (controlName == "PauseButton")
        return String("pause playback");
    if (controlName == "Slider")
        return String("movie time scrubber");
    if (controlName == "SliderThumb")
        return String("movie time scrubber thumb");
    if (controlName == "RewindButton")
        return String("seek movie back 30 seconds");
    if (controlName == "ReturnToRealtimeButton")
        return String("return streaming movie to real time");
    if (controlName == "CurrentTimeDisplay")
        return String("current movie time in seconds");
    if (controlName == "TimeRemainingDisplay")
        return String("number of seconds of movie remaining");
    if (controlName == "StatusDisplay")
        return String("current movie status");
    if (controlName == "SeekBackButton")
        return String("seek quickly back");
    if (controlName == "SeekForwardButton")
        return String("seek quickly forward");
    if (controlName == "FullscreenButton")
        return String("Play movie in fullscreen mode");
    if (controlName == "ShowClosedCaptionsButton")
        return String("start displaying closed captions");
    if (controlName == "HideClosedCaptionsButton")
        return String("stop displaying closed captions");

    ASSERT_NOT_REACHED();
    return String();
}

String localizedMediaTimeDescription(const String& controlName)
{
    notImplemented();
    return String();
}
#endif  // ENABLE(VIDEO)

String validationMessageValueMissingForCheckboxText()
{
    notImplemented();
    return validationMessageValueMissingText();
}

String validationMessageValueMissingForFileText()
{
    notImplemented();
    return validationMessageValueMissingText();
}

String validationMessageValueMissingForMultipleFileText()
{
    notImplemented();
    return validationMessageValueMissingText();
}

String validationMessageValueMissingForRadioText()
{
    notImplemented();
    return validationMessageValueMissingText();
}

String validationMessageValueMissingForSelectText()
{
    notImplemented();
    return validationMessageValueMissingText();
}
String searchableIndexIntroduction()
{
    notImplemented();
    return String("This is a searchable index. Enter search keywords: ");
}

String resetButtonDefaultLabel()
{
    return String("Reset");
}

String defaultDetailsSummaryText()
{
    return String("Details");
}

String submitButtonDefaultLabel()
{
    return String("Submit");
}

String inputElementAltText()
{
    notImplemented();
    return String("Submit");
}

String validationMessageValueMissingText()
{
    notImplemented();
    return String("value missing");
}

String validationMessageTypeMismatchText()
{
    notImplemented();
    return String("type mismatch");
}

String validationMessageTypeMismatchForEmailText()
{
    notImplemented();
    return validationMessageTypeMismatchText();
}

String validationMessageTypeMismatchForMultipleEmailText()
{
    notImplemented();
    return validationMessageTypeMismatchText();
}

String validationMessageTypeMismatchForURLText()
{
    notImplemented();
    return validationMessageTypeMismatchText();
}

String validationMessagePatternMismatchText()
{
    notImplemented();
    return String("pattern mismatch");
}

String validationMessageTooLongText(int, int)
{
    notImplemented();
    return String("too long");
}

String validationMessageRangeUnderflowText(const String&)
{
    return String("range underflow");
}

String validationMessageRangeOverflowText(const String&)
{
    notImplemented();
    return String("range overflow");
}

String validationMessageStepMismatchText(const String&, const String&)
{
    return String("step mismatch");
}

String localizedMediaTimeDescription(float time)
{
    notImplemented();
    return String("media time description");
}

String contextMenuItemTagOpenVideoInNewWindow()
{
    return String("Open _Video in New Window");
}

String contextMenuItemTagOpenAudioInNewWindow()
{
    return String("Open _Audio in New Window");
}

String contextMenuItemTagCopyVideoLinkToClipboard()
{
    return String("Cop_y Video Link Location");
}

String contextMenuItemTagCopyAudioLinkToClipboard()
{
    return String("Cop_y Audio Link Location");
}

String contextMenuItemTagToggleMediaControls()
{
    return String("_Toggle Media Controls");
}

String contextMenuItemTagToggleMediaLoop()
{
    return String("Toggle Media _Loop Playback");
}

String contextMenuItemTagEnterVideoFullscreen()
{
    return String("Switch Video to _Fullscreen");
}

String contextMenuItemTagMediaPlay()
{
    return String("_Play");
}

String contextMenuItemTagMediaPause()
{
    return String("_Pause");
}

String contextMenuItemTagMediaMute()
{
    return String("_Mute");
}

} // namespace WebCore
