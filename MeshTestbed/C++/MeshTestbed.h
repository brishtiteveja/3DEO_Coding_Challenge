//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MeshTestbed.h : main header file for the MESHTESTBED application
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHTESTBED_H__3205A67D_41BB_4C1F_9E61_0DE2CD71F8F3__INCLUDED_)
#define AFX_MESHTESTBED_H__3205A67D_41BB_4C1F_9E61_0DE2CD71F8F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedApp:
// See MeshTestbed.cpp for the implementation of this class
//

class CMeshTestbedApp : public CWinApp
{
public:
	CMeshTestbedApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshTestbedApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMeshTestbedApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CMeshTestbedApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHTESTBED_H__3205A67D_41BB_4C1F_9E61_0DE2CD71F8F3__INCLUDED_)
