// NLB_Bldr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NLB_Bldr.h"

#include "MainFrm.h"
#include "NLB_BldrDoc.h"
#include "NLB_BldrView.h"
#include "consts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrApp

BEGIN_MESSAGE_MAP(CNLB_BldrApp, CWinApp)
	//{{AFX_MSG_MAP(CNLB_BldrApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrApp construction

CNLB_BldrApp::CNLB_BldrApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNLB_BldrApp object

CNLB_BldrApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrApp initialization

BOOL CNLB_BldrApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNLB_BldrDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNLB_BldrView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Register our Clipboard format names.
    m_uiNLBClipFormat = ::RegisterClipboardFormat(SZ_NLB_CLIP_FORMAT);
    

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CNLB_BldrApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrApp message handlers

void CNLB_BldrApp::HandleError(int iErrCode)
{
	CString strError;
	CString strCapt;

	strCapt.LoadString(IDS_ERROR_MB_CAPT);
	switch (iErrCode)
	{
	case IERR_OUT_OF_MEMORY:
		{			
			strError.LoadString(IDS_ERR_OUT_OF_MEMORY);			
			MessageBox(NULL, strError, strCapt, MB_ICONERROR);
			exit(1);
		}
		break;

	case IERR_CLIPBOARD_ERROR:
		{			
			strError.LoadString(IDS_ERR_CLIPBOARD_ERROR);			
			MessageBox(NULL, strError, strCapt, MB_ICONERROR);
			exit(1);
		}
		break;


	case IERR_FILE_CORRUPT:
		{			
			strError.LoadString(IDS_ERR_FILE_CORRUPT);			
			MessageBox(NULL, strError, strCapt, MB_ICONERROR);
			exit(1);
		}
		break;

	case IERR_UNKNOWN_ERROR_WHILE_PROCESSING_TRIGGERED_STRING_VAR:
		{			
			strError.LoadString(IDS_ERR_UNKNOWN_ERROR_WHILE_PROCESSING_TRIGGERED_STRING_VAR);			
			MessageBox(NULL, strError, strCapt, MB_ICONERROR);			
		}
		break;

	default:
		{			
			strError.LoadString(IDS_ERR_UNKNOWN);			
			MessageBox(NULL, strError, strCapt, MB_ICONERROR);
			exit(1);
		}
		break;
	}
}
