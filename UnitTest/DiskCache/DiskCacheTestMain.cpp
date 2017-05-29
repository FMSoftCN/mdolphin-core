#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "TestClient.h"

using namespace UnitTest;

#define IDC_DISKCACHEPUSH  	100
#define IDC_DISKCACHELIST   101

static void InitListView(HWND hListView);
static void InsertListViewItem(HWND hListView, int itemIndex, TestResult *pTestResult);

static TestClient* m_pTestClient = NULL;
static int m_iItemIndex = 0;
static HWND m_hListView = NULL;

static DLGTEMPLATE DlgInitProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    40, 12, 720, 576, 
    "The system is initializing",
    0, 0,
    0, NULL,
    0
};

typedef struct _ColumnInfo {
    const char* col_name;
    int col_width;
}ColumnInfo;

static ColumnInfo m_aColumnInfo[] = {
    { "FilePath", 150 },
    { "FileLine", 50 },
    { "ClassType", 80 },
    { "FuntionName", 100 },
    { "Message", 200 },
    { "RealTestResult", 100 },
    { "ExpectTestResult", 100 },
};

static CTRLDATA CtrlInit [] =
{ 
    {
        "listview",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LVS_TREEVIEW,
        10, 30, 690, 450,
        IDC_DISKCACHELIST,
        "",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHLIKE,
        400, 500, 60, 25,
        IDC_DISKCACHEPUSH,
        "DiskCache",
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        490, 500, 60, 25,
        IDOK, 
        "OK",
        0
    }
};

static int ShowDataCallBack(TestResult *pTestResult)
{
    //DEBUG_OUTPUT("Enter");
    InsertListViewItem(m_hListView, m_iItemIndex++, pTestResult);
    return 0;
}

static void InitListView(HWND hListView)
{
    int i;
    LVCOLUMN lvcol;

    for(i = 0; i < (int)sizeof(m_aColumnInfo) / sizeof(*m_aColumnInfo); i++) {
        lvcol.nCols = i;
        lvcol.pszHeadText = const_cast<char*>(m_aColumnInfo[i].col_name);
        lvcol.width = m_aColumnInfo[i].col_width;
        lvcol.pfnCompare = NULL;
        lvcol.colFlags = 0;
        if(hListView) {
            SendMessage (hListView, LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
        }
    }

    
}

static void InsertListViewItem(HWND hListView, int itemIndex, TestResult *pTestResult)
{
    int i;
    LVITEM item;
    GHANDLE hitem = 0;
    LVSUBITEM subdata;
    if(0 == hListView)
        return;


    item.dwFlags &= ~LVIF_FOLD;
    item.nItemHeight = 25;

    SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0); 

    hitem = SendMessage (hListView, LVM_ADDITEM, 0, (LPARAM)&item);

    for(i = 0; i < (int)sizeof(m_aColumnInfo) / sizeof(*m_aColumnInfo); i++) {
        subdata.nItem = itemIndex;
        subdata.subItem = i;

        if (i == 0)
            subdata.pszText = pTestResult->file_name;
        else if(i == 1)
            subdata.pszText = pTestResult->file_line;
        else if(i == 2)
            subdata.pszText = pTestResult->class_type;
        else if(i == 3) 
            subdata.pszText = pTestResult->fun_name;
        else if(i == 4) 
            subdata.pszText = pTestResult->msg;
        else if(i == 5)
            subdata.pszText = pTestResult->real_result;
        else
            subdata.pszText = pTestResult->expect_result;

        subdata.nTextColor = PIXEL_black;
        subdata.flags = 0;
        subdata.image = 0;
        SendMessage (hListView, LVM_SETSUBITEM, hitem, (LPARAM) &subdata);
    }

    SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
}


static int InitDialogBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hListView = GetDlgItem (hDlg, IDC_DISKCACHELIST);
    m_hListView = hListView;
    switch (message) {
    case MSG_INITDIALOG:
        m_iItemIndex = 0;

        InitListView(hListView);

        if(!m_pTestClient)
            m_pTestClient = new TestClient(ShowDataCallBack);

        return 1;
        
    case MSG_COMMAND:
        switch (wParam) {
        case IDC_DISKCACHEPUSH:
            m_iItemIndex = 0;
            SendMessage(hListView, LVM_DELALLITEM, 0, 0);
            if(m_pTestClient)
                m_pTestClient->doTest();
            break;
        case IDOK:
        case IDCANCEL:
            PostMessage(hDlg, MSG_CLOSE, 0, 0);
            break;
        default:
            DEBUG_OUTPUT("wParam:%d", wParam);
            break;
        }
        break;

    case MSG_CLOSE:
        if(m_pTestClient) {
            delete m_pTestClient;
            m_pTestClient = NULL;
        }
        m_hListView = NULL;
        m_iItemIndex = 0;
        EndDialog (hDlg, IDCANCEL);
        break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static void InitDialogBox (HWND hWnd)
{
    DlgInitProgress.controls = CtrlInit;
    DlgInitProgress.controlnr = TABLESIZE(CtrlInit);
    
    DialogBoxIndirectParam (&DlgInitProgress, hWnd, InitDialogBoxProc, 0L);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "dialogbox" , 0 , 0);
#endif
    
    InitDialogBox (HWND_DESKTOP);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

