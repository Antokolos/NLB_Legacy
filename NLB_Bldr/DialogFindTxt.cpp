// DialogFindTxt.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "DialogFindTxt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogFindTxt dialog


CDialogFindTxt::CDialogFindTxt(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFindTxt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogFindTxt)
	m_fFindInBookVars = FALSE;
	m_fFindInLnks = FALSE;
	m_fFindInPages = FALSE;
	m_fFindInProbVars = FALSE;
	m_strToFind = _T("");
	m_fCaseSensitive = FALSE;
	m_fWholeWords = FALSE;
	//}}AFX_DATA_INIT

	m_fFindInBookVars = F_FIND_IN_BOOKVARS;
	m_fFindInLnks = F_FIND_IN_LNKS;
	m_fFindInPages = F_FIND_IN_PAGES;
	m_fFindInProbVars = F_FIND_IN_PROBVARS;
	m_fCaseSensitive = F_CASE_SENSITIVE;
	m_fWholeWords = F_WHOLE_WORDS;	
}


void CDialogFindTxt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFindTxt)
	DDX_Check(pDX, IDC_CHECK_FIND_IN_BOOKVARS, m_fFindInBookVars);
	DDX_Check(pDX, IDC_CHECK_FIND_IN_LNKS, m_fFindInLnks);
	DDX_Check(pDX, IDC_CHECK_FIND_IN_PAGES, m_fFindInPages);
	DDX_Check(pDX, IDC_CHECK_FIND_IN_PROBVARS, m_fFindInProbVars);
	DDX_Text(pDX, IDC_EDIT_STR_TO_FIND, m_strToFind);
	DDX_Check(pDX, IDC_CHECK_CASE_SENSITIVE, m_fCaseSensitive);
	DDX_Check(pDX, IDC_CHECK_WHOLE_WORDS, m_fWholeWords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFindTxt, CDialog)
	//{{AFX_MSG_MAP(CDialogFindTxt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFindTxt message handlers
