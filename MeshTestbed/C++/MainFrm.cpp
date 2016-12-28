//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MainFrm.cpp : implementation of the CMainFrame class
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshTestbed.h"

#include "DemoInfo.h"
#include "GLView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_wndProgressCtrl = NULL;
	m_wndDemoInfoBar = NULL;
}

CMainFrame::~CMainFrame()
{
	if(m_wndProgressCtrl != NULL) delete m_wndProgressCtrl;
	if(m_wndDemoInfoBar != NULL) delete m_wndDemoInfoBar;
}

CProgressCtrl *CMainFrame::CreateProgressBar(void)
{
	if (m_wndProgressCtrl == NULL)
	{
		RECT rc;
		m_wndStatusBar.GetItemRect(0, &rc);
		m_wndProgressCtrl = new CProgressCtrl; 
		
		VERIFY (m_wndProgressCtrl->Create(WS_CHILD | WS_VISIBLE, rc, &m_wndStatusBar, 1));
	
	}
	m_wndProgressCtrl->SetRange(0, 100);
	m_wndProgressCtrl->SetStep(1);
    
	return(m_wndProgressCtrl);
}

void CMainFrame::DestroyProgressBar(void)
{
	if (m_wndProgressCtrl != NULL)
	{
		delete m_wndProgressCtrl;
		m_wndProgressCtrl = NULL;
	}
 }

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// ViewOptions Toolbar
	if (!m_wndViewOptionsBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || 
		!m_wndViewOptionsBar.LoadToolBar(IDR_VIEWTOOL))
	{
		TRACE0("Failed to create ViewTool toolbar\n");
		return -1;      // fail to create
	}
	m_wndViewOptionsBar.SetWindowText("Views");
	m_wndViewOptionsBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndViewOptionsBar);

	// truss structure info. Dialog bar
	m_wndDemoInfoBar = new CDemoInfo();
	if(!m_wndDemoInfoBar->Create(this, IDD_DIALOG_DEMOINFO, CBRS_LEFT, IDD_DIALOG_DEMOINFO))
	{
		TRACE0("Failed to create demo Info toolbar\n");
		return -1;      // fail to create
	}
	m_wndDemoInfoBar->SetWindowText("Demo Info");
	m_wndDemoInfoBar->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);

	m_wndDemoInfoBar->InitValues();

	return 0;
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
	CFrameWnd::OnPaletteChanged(pFocusWnd);

	if(pFocusWnd != this)
		OnQueryNewPalette();
}

BOOL CMainFrame::OnQueryNewPalette()
{
	int        i;
	CPalette    *pOldPal;
	CGLView   *pView = (CGLView *)GetActiveView();
	CClientDC   dc(pView);


	pOldPal = dc.SelectPalette(&pView->m_cPalette, FALSE);
	i = dc.RealizePalette();
	dc.SelectPalette(pOldPal, FALSE);

	if(i > 0)
		InvalidateRect(NULL);

	return CFrameWnd::OnQueryNewPalette();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}
