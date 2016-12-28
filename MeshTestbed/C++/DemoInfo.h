//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOINFO_H__9D72CD23_861F_4B29_8824_072D7EFC7B61__INCLUDED_)
#define AFX_DEMOINFO_H__9D72CD23_861F_4B29_8824_072D7EFC7B61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DemoInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDemoInfo dialog
class CVertex;
enum RadioType { RADIO1, RADIO2, RADIO3 };

class CDemoInfo : public CDialogBar
{
	DECLARE_DYNAMIC(CDemoInfo)

// Construction
public:
	CDemoInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoInfo)
	enum { IDD = IDD_DIALOG_DEMOINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	double mEditVertNum;
	double mEditTriNum;
	int mEditHandleNum;
	double mEditExtX, mEditExtY, mEditExtZ;

	bool bCheck1;
	bool bCheck2;


protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CDemoInfo)
	virtual BOOL InitValues();
	RadioType GetRadioType();
	void GetEditVertNumValue(double &Edit1Value);	
	void GetEditTriNumValue(double &Edit2Value);	
	void GetEditHandleNumValue(double &EditHandleNumValue);
	void GetEditExtValues(double *PartExt);

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOINFO_H__9D72CD23_861F_4B29_8824_072D7EFC7B61__INCLUDED_)
