//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2ED07088_CE1D_401C_B6EE_9D16380AFBA0__INCLUDED_)
#define AFX_MAINFRM_H__2ED07088_CE1D_401C_B6EE_9D16380AFBA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemoInfo;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CProgressCtrl *m_wndProgressCtrl;
	CDemoInfo *m_wndDemoInfoBar;

// Operations
public:
	CProgressCtrl *CreateProgressBar(void);
	void DestroyProgressBar(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndViewOptionsBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2ED07088_CE1D_401C_B6EE_9D16380AFBA0__INCLUDED_)
