/* 
** $Id$
** 
** PlatformKeyboardEventMg.cpp: class PlatformKeyboardEvent's natvie implemention
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: dongjunjie
**
** Create date: 2010-06-01
*/


#include "minigui.h"
#include "config.h"
#include "PlatformKeyboardEvent.h"
#include "WindowsKeyboardCodes.h"
#include <wtf/ASCIICType.h>


namespace WebCore {

static int windowsKeyCodeForKeyEvent(unsigned int keycode)
{
    switch (keycode) {
        case SCANCODE_KEYPAD0:
            return VK_NUMPAD0;// (60) Numeric keypad 0 key
        case SCANCODE_KEYPAD1:
            return VK_NUMPAD1;// (61) Numeric keypad 1 key
        case SCANCODE_KEYPAD2:
            return VK_NUMPAD2; // (62) Numeric keypad 2 key
        case SCANCODE_KEYPAD3:
            return VK_NUMPAD3; // (63) Numeric keypad 3 key
        case SCANCODE_KEYPAD4:
            return VK_NUMPAD4; // (64) Numeric keypad 4 key
        case SCANCODE_KEYPAD5:
            return VK_NUMPAD5; //(65) Numeric keypad 5 key
        case SCANCODE_KEYPAD6:
            return VK_NUMPAD6; // (66) Numeric keypad 6 key
        case SCANCODE_KEYPAD7:
            return VK_NUMPAD7; // (67) Numeric keypad 7 key
        case SCANCODE_KEYPAD8:
            return VK_NUMPAD8; // (68) Numeric keypad 8 key
        case SCANCODE_KEYPAD9:
            return VK_NUMPAD9; // (69) Numeric keypad 9 key
        case SCANCODE_KEYPADMULTIPLY:
            return VK_MULTIPLY; // (6A) Multiply key
        case SCANCODE_KEYPADPLUS:
            return VK_ADD; // (6B) Add key
        case SCANCODE_KEYPADMINUS:
            return VK_SUBTRACT; // (6D) Subtract key
        /*
        case SCANCODE_KEYPADPERIOD:
            return VK_DECIMAL; // (6E) Decimal key
        case SCANCODE_KEYPADDIVIDE:
            return VK_DIVIDE; // (6F) Divide key
            */

        case SCANCODE_BACKSPACE:
            return VK_BACK; // (08) BACKSPACE key
        case SCANCODE_TAB:
            return VK_TAB; // (09) TAB key
            /*
        case SCANCODE_Clear:
            return VK_CLEAR; // (0C) CLEAR key
            */
        case SCANCODE_ENTER:
            return VK_RETURN; //(0D) Return key
        case SCANCODE_LEFTSHIFT:
        case SCANCODE_RIGHTSHIFT:
            return VK_SHIFT; // (10) SHIFT key
        case SCANCODE_LEFTCONTROL:
        case SCANCODE_RIGHTCONTROL:
            return VK_CONTROL; // (11) CTRL key
        case SCANCODE_MENU:
        case SCANCODE_LEFTALT:
        case SCANCODE_RIGHTALT:
            return VK_MENU; // (12) ALT key

        case SCANCODE_PAUSE:
            return VK_PAUSE; // (13) PAUSE key
        case SCANCODE_CAPSLOCK:
            return VK_CAPITAL; // (14) CAPS LOCK key
            /*
        case SCANCODE_Kana_Lock:
        case SCANCODE_Kana_Shift:
            return VK_KANA; // (15) Input Method Editor (IME) Kana mode
        case SCANCODE_Hangul:
            return VK_HANGUL; // VK_HANGUL (15) IME Hangul mode
            // VK_JUNJA (17) IME Junja mode
            // VK_FINAL (18) IME final mode
        case SCANCODE_Hangul_Hanja:
            return VK_HANJA; // (19) IME Hanja mode
        case SCANCODE_Kanji:
            return VK_KANJI; // (19) IME Kanji mode
            */
        case SCANCODE_ESCAPE:
            return VK_ESCAPE; // (1B) ESC key
            // VK_CONVERT (1C) IME convert
            // VK_NONCONVERT (1D) IME nonconvert
            // VK_ACCEPT (1E) IME accept
            // VK_MODECHANGE (1F) IME mode change request
        case SCANCODE_SPACE:
            return VK_SPACE; // (20) SPACEBAR
        case SCANCODE_PAGEUP:
            return VK_PRIOR; // (21) PAGE UP key
        case SCANCODE_PAGEDOWN:
            return VK_NEXT; // (22) PAGE DOWN key
        case SCANCODE_END:
            return VK_END; // (23) END key
        case SCANCODE_HOME:
            return VK_HOME; // (24) HOME key
        case SCANCODE_CURSORBLOCKLEFT:
            return VK_LEFT; // (25) LEFT ARROW key
        case SCANCODE_CURSORBLOCKUP:
            return VK_UP; // (26) UP ARROW key
        case SCANCODE_CURSORBLOCKRIGHT:
            return VK_RIGHT; // (27) RIGHT ARROW key
        case SCANCODE_CURSORBLOCKDOWN:
            return VK_DOWN; // (28) DOWN ARROW key
            /*
        case SCANCODE_Select:
            return VK_SELECT; // (29) SELECT key
            */
        case SCANCODE_PRINTSCREEN:
            return VK_PRINT; // (2A) PRINT key
            /*
        case SCANCODE_Execute:
            return VK_EXECUTE;// (2B) EXECUTE key
            */
            //dunno on this
            //case SCANCODE_PrintScreen:
            //      return VK_SNAPSHOT; // (2C) PRINT SCREEN key
        case SCANCODE_INSERT:
            return VK_INSERT; // (2D) INS key
        case SCANCODE_REMOVE:
            return VK_DELETE; // (2E) DEL key
            /*
        case SCANCODE_Help:
            return VK_HELP; // (2F) HELP key
            */
        case SCANCODE_0:
            return VK_0;    //  (30) 0) key
        case SCANCODE_1:
            return VK_1; //  (31) 1 ! key
        case SCANCODE_2:
            return VK_2; //  (32) 2 & key
        case SCANCODE_3:
            return VK_3; //case '3': case '#';
        case SCANCODE_4:
            return VK_4;
        case SCANCODE_5:
            return VK_5; //  (35) 5 key  '%'
        case SCANCODE_6:
            return VK_6; //  (36) 6 key  '^'
        case SCANCODE_7:
            return VK_7; //  (37) 7 key  case '&'
        case SCANCODE_8:
            return VK_8; //  (38) 8 key  '*'
        case SCANCODE_9:
            return VK_9; //  (39) 9 key '('
        case SCANCODE_A:
            return VK_A; //  (41) A key case 'a': case 'A': return 0x41;
        case SCANCODE_B:
            return VK_B; //  (42) B key case 'b': case 'B': return 0x42;
        case SCANCODE_C:
            return VK_C; //  (43) C key case 'c': case 'C': return 0x43;
        case SCANCODE_D:
            return VK_D; //  (44) D key case 'd': case 'D': return 0x44;
        case SCANCODE_E:
            return VK_E; //  (45) E key case 'e': case 'E': return 0x45;
        case SCANCODE_F:
            return VK_F; //  (46) F key case 'f': case 'F': return 0x46;
        case SCANCODE_G:
            return VK_G; //  (47) G key case 'g': case 'G': return 0x47;
        case SCANCODE_H:
            return VK_H; //  (48) H key case 'h': case 'H': return 0x48;
        case SCANCODE_I:
            return VK_I; //  (49) I key case 'i': case 'I': return 0x49;
        case SCANCODE_J:
            return VK_J; //  (4A) J key case 'j': case 'J': return 0x4A;
        case SCANCODE_K:
            return VK_K; //  (4B) K key case 'k': case 'K': return 0x4B;
        case SCANCODE_L:
            return VK_L; //  (4C) L key case 'l': case 'L': return 0x4C;
        case SCANCODE_M:
            return VK_M; //  (4D) M key case 'm': case 'M': return 0x4D;
        case SCANCODE_N:
            return VK_N; //  (4E) N key case 'n': case 'N': return 0x4E;
        case SCANCODE_O:
            return VK_O; //  (4F) O key case 'o': case 'O': return 0x4F;
        case SCANCODE_P:
            return VK_P; //  (50) P key case 'p': case 'P': return 0x50;
        case SCANCODE_Q:
            return VK_Q; //  (51) Q key case 'q': case 'Q': return 0x51;
        case SCANCODE_R:
            return VK_R; //  (52) R key case 'r': case 'R': return 0x52;
        case SCANCODE_S:
            return VK_S; //  (53) S key case 's': case 'S': return 0x53;
        case SCANCODE_T:
            return VK_T; //  (54) T key case 't': case 'T': return 0x54;
        case SCANCODE_U:
            return VK_U; //  (55) U key case 'u': case 'U': return 0x55;
        case SCANCODE_V:
            return VK_V; //  (56) V key case 'v': case 'V': return 0x56;
        case SCANCODE_W:
            return VK_W; //  (57) W key case 'w': case 'W': return 0x57;
        case SCANCODE_X:
            return VK_X; //  (58) X key case 'x': case 'X': return 0x58;
        case SCANCODE_Y:
            return VK_Y; //  (59) Y key case 'y': case 'Y': return 0x59;
        case SCANCODE_Z:
            return VK_Z; //  (5A) Z key case 'z': case 'Z': return 0x5A;
        case SCANCODE_LEFTWIN:
            return VK_LWIN; // (5B) Left Windows key (Microsoft Natural keyboard)
        case SCANCODE_RIGHTWIN:
            return VK_RWIN; // (5C) Right Windows key (Natural keyboard)
            // VK_APPS (5D) Applications key (Natural keyboard)
            // VK_SLEEP (5F) Computer Sleep key
            // VK_SEPARATOR (6C) Separator key
            // VK_SUBTRACT (6D) Subtract key
            // VK_DECIMAL (6E) Decimal key
            // VK_DIVIDE (6F) Divide key
            // handled by key code above

        case SCANCODE_NUMLOCK:
            return VK_NUMLOCK; // (90) NUM LOCK key

        case SCANCODE_SCROLLLOCK:
            return VK_SCROLL; // (91) SCROLL LOCK key

            // VK_LSHIFT (A0) Left SHIFT key
            // VK_RSHIFT (A1) Right SHIFT key
            // VK_LCONTROL (A2) Left CONTROL key
            // VK_RCONTROL (A3) Right CONTROL key
            // VK_LMENU (A4) Left MENU key
            // VK_RMENU (A5) Right MENU key
            // VK_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
            // VK_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
            // VK_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
            // VK_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
            // VK_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
            // VK_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
            // VK_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
            // VK_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
            // VK_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
            // VK_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
            // VK_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
            // VK_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
            // VK_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
            // VK_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
            // VK_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
            // VK_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
            // VK_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
            // VK_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key

            // VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
        case SCANCODE_SEMICOLON:
            return VK_OEM_1; //case ';': case ':': return 0xBA;
            // VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
        case SCANCODE_EQUAL:
            return VK_OEM_PLUS; //case '=': case '+': return 0xBB;
            // VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
        case SCANCODE_LESS:
            return VK_OEM_COMMA; //case ',': case '<': return 0xBC;
            // VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
        case SCANCODE_MINUS:
            return VK_OEM_MINUS; //case '-': case '_': return 0xBD;
            // VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
        case SCANCODE_PERIOD:
            return VK_OEM_PERIOD; //case '.': case '>': return 0xBE;
            // VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
        case SCANCODE_SLASH:
            return VK_OEM_2; //case '/': case '?': return 0xBF;
            // VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
        case SCANCODE_GRAVE:
            return VK_OEM_3; //case '`': case '~': return 0xC0;
            // VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
        case SCANCODE_BRACKET_LEFT:
            return VK_OEM_4; //case '[': case '{': return 0xDB;
            // VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
        case SCANCODE_BACKSLASH:
            return VK_OEM_5; //case '\\': case '|': return 0xDC;
            // VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
        case SCANCODE_BRACKET_RIGHT:
            return VK_OEM_6; // case ']': case '}': return 0xDD;
            // VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
        case SCANCODE_APOSTROPHE:
            return VK_OEM_7; // case '\'': case '"': return 0xDE;
            // VK_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
            // VK_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
            // VK_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
            // VK_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
            // VK_ATTN (F6) Attn key
            // VK_CRSEL (F7) CrSel key
            // VK_EXSEL (F8) ExSel key
            // VK_EREOF (F9) Erase EOF key
            // VK_PLAY (FA) Play key
            // VK_ZOOM (FB) Zoom key
            // VK_NONAME (FC) Reserved for future use
            // VK_PA1 (FD) PA1 key
            // VK_OEM_CLEAR (FE) Clear key
            case SCANCODE_F1:
                    return VK_F1;
            case SCANCODE_F2:
                    return VK_F2;
            case SCANCODE_F3:
                    return VK_F3;
            case SCANCODE_F4:
                    return VK_F4;
            case SCANCODE_F5:
                    return VK_F5;
            case SCANCODE_F6:
                    return VK_F6;
            case SCANCODE_F7:
                    return VK_F7;
            case SCANCODE_F8:
                    return VK_F8;
            case SCANCODE_F9:
                    return VK_F9;
            case SCANCODE_F10:
                    return VK_F10;
            case SCANCODE_F11:
                    return VK_F11;
            case SCANCODE_F12:
                    return VK_F12;
        default:
            return 0;
    }

}

static int charCodeToKeyEvent(unsigned int charCode)
{

    switch (charCode) {
        case '\r':
            return VK_RETURN; //(0D) Return key
        default:
            return 0; //(0D) Return key
    }
}

static char scancodeToChar(int scancode)
{
    //scancode is according the standared keyboard, from left-top to right-bottom: 
    //f.g ESC ` 1 2 3 4 5 6 .... Q W E R T Y ... B N M , . /
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

    scancode -= 1;
    if(scancode < 0 || scancode >= static_cast<int>(sizeof(ascii_maps)))
        return 0;
    return ascii_maps[scancode];
}

static String keyIdentifierForWindowsKeyCode(unsigned short keyCode)
{
    switch (keyCode) {
        case SCANCODE_MENU:
            return "Alt";
		case SCANCODE_TAB:
			return "U+0009";
		case SCANCODE_CURSORDOWN:
            return "Down";
        case SCANCODE_END:
            return "End";
        case SCANCODE_ENTER:
            return "Enter";
        case SCANCODE_SPACE:
            return "Space";
		case SCANCODE_LEFTSHIFT:
		case SCANCODE_RIGHTSHIFT:
			return "Shift";
        case SCANCODE_F1:
            return "F1";
        case SCANCODE_F2:
            return "F2";
        case SCANCODE_F3:
            return "F3";
        case SCANCODE_F4:
            return "F4";
        case SCANCODE_F5:
            return "F5";
        case SCANCODE_F6:
            return "F6";
        case SCANCODE_F7:
            return "F7";
        case SCANCODE_F8:
            return "F8";
        case SCANCODE_F9:
            return "F9";
        case SCANCODE_F10:
            return "F11";
        case SCANCODE_F12:
            return "F12";
        case SCANCODE_HOME:
            return "Home";
        case SCANCODE_INSERT:
            return "Insert";
        case SCANCODE_CURSORLEFT:
            return "Left";
        case SCANCODE_PAGEDOWN:
            return "PageDown";
        case SCANCODE_PAGEUP:
            return "PageUp";
        case SCANCODE_PAUSE:
            return "Pause";
        case SCANCODE_CURSORRIGHT:
            return "Right";
        case SCANCODE_CURSORUP:
            return "Up";
        case SCANCODE_BACKSPACE:
            return "U+0008";
        case SCANCODE_REMOVE:
            return "U+007F";
        case SCANCODE_CURSORBLOCKLEFT:
            return "Left";
        case SCANCODE_CURSORBLOCKRIGHT:
            return "Right";
        case SCANCODE_CURSORBLOCKUP:
            return "Up";
        case SCANCODE_CURSORBLOCKDOWN:
            return "Down";
        default:
            return String::format("U+%04X", WTF::toASCIIUpper(scancodeToChar(keyCode)));
    }
}
static String singleCharacterString(unsigned int keyCode) 
{
   // UChar c = (UChar)windowsKeyCodeForKeyEvent(keyCode);
   UChar c = (UChar)scancodeToChar(keyCode);
    return String(&c, 1); 
}

PlatformKeyboardEvent::PlatformKeyboardEvent(HWND hWnd, int message, WPARAM wParam, LPARAM lParam, String text)
    : m_type(Char)
	, m_text(text)
    , m_unmodifiedText(text)
    , m_keyIdentifier(String())
    , m_autoRepeat(((lParam & KS_REPEATED) ? true : false))
    , m_windowsVirtualKeyCode(charCodeToKeyEvent(wParam))
	, m_nativeVirtualKeyCode(wParam)
    , m_isKeypad(false) // FIXME: don't use keypad
    , m_shiftKey( (GetShiftKeyStatus() & KS_SHIFT) || (lParam & KS_SHIFT) )
    , m_ctrlKey( (GetShiftKeyStatus() & KS_CTRL) || (lParam & KS_CTRL) )
    , m_altKey( (GetShiftKeyStatus() & KS_ALT) || (lParam & KS_ALT) )
    , m_metaKey(m_altKey)
{

}

PlatformKeyboardEvent::PlatformKeyboardEvent(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
    : m_type(message == MSG_KEYDOWN ? RawKeyDown : KeyUp)
	, m_text(singleCharacterString(wParam))
    , m_unmodifiedText(singleCharacterString(wParam))
    , m_keyIdentifier(keyIdentifierForWindowsKeyCode(wParam))
    , m_autoRepeat(((lParam & KS_REPEATED) ? true : false))
    , m_windowsVirtualKeyCode(windowsKeyCodeForKeyEvent(wParam))
	, m_nativeVirtualKeyCode(wParam)
    , m_isKeypad(false) // FIXME: don't use keypad
    , m_shiftKey( (GetShiftKeyStatus() & KS_SHIFT) || (lParam & KS_SHIFT) )
    , m_ctrlKey( (GetShiftKeyStatus() & KS_CTRL) || (lParam & KS_CTRL) )
    , m_altKey( (GetShiftKeyStatus() & KS_ALT) || (lParam & KS_ALT) )
    , m_metaKey(m_altKey)
{

}

bool PlatformKeyboardEvent::currentCapsLockState()
{
	return GetKeyStatus(KS_CAPSLOCK);
}

void PlatformKeyboardEvent::getCurrentModifierState(bool& shiftKey, bool& ctrlKey, bool& altKey, bool& metaKey)
{
	shiftKey  = GetKeyStatus(KS_SHIFT);
	ctrlKey   = GetKeyStatus(KS_CTRL);
	altKey    = GetKeyStatus(KS_ALT);
	metaKey   = altKey;
}

void PlatformKeyboardEvent::disambiguateKeyDownEvent(Type type, bool backwardCompatibilityMode /*= false*/)
{
    ASSERT(m_type == KeyDown);
    m_type = type;
    if (backwardCompatibilityMode)
        return;

    if (type == RawKeyDown) {
        m_text = String();
        m_unmodifiedText = String();
    } else {
        m_keyIdentifier = String();
        m_windowsVirtualKeyCode = 0;
    }
}

}

