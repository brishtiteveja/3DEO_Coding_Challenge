//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MeshTestbedDoc.h : interface of the CMeshTestbedDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHTESTBEDDOC_H__910EC711_51DC_4347_917E_77A93E8DD899__INCLUDED_)
#define AFX_MESHTESTBEDDOC_H__910EC711_51DC_4347_917E_77A93E8DD899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vertex.h"
#include "triangle.h"
#include "SolidSTL.h"

class CMeshTestbedView;

#define MAXIMUM_PARTS 10

class CMeshTestbedDoc : public CDocument
{
protected: // create from serialization only
	CMeshTestbedDoc();
	DECLARE_DYNCREATE(CMeshTestbedDoc)

// Attributes
public:
	int m_PartNum;
	SolidSTL *mPart[MAXIMUM_PARTS];

	double m_minExt[3], m_maxExt[3];

	// HW4
	double stepSize;
	CVertex Bezier_CtrlV[4];
	int BezierVNum;
	CVertex *BezierCurv;

	CVertex BezierSurf_CtrlV[4][4];
	int BezierSurfVNumX, BezierSurfVNumY;
	int BezierSurfVNum;
	CVertex *BezierSurf;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshTestbedDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CMeshTestbedView *GetView(void);
	void OpenSTL(int aPartNum);
	void GetBoundingBox(double *PartsMin, double *PartsMax);

// Implementation
public:
	virtual ~CMeshTestbedDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMeshTestbedDoc)
	afx_msg void OnFileOpenSTL1();
	afx_msg void OnFileOpenSTL2();
	afx_msg void OnFileOpenSTL3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHTESTBEDDOC_H__910EC711_51DC_4347_917E_77A93E8DD899__INCLUDED_)
