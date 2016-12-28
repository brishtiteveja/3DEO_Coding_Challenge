//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MeshTestbedView.h : interface of the CMeshTestbedView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHTESTBEDVIEW_H__FE0AF8B0_918D_4C84_B0B5_AE0753417E57__INCLUDED_)
#define AFX_MESHTESTBEDVIEW_H__FE0AF8B0_918D_4C84_B0B5_AE0753417E57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLView.h"

class CMeshTestbedDoc;
class SolidSTL_Viewer;

class CMeshTestbedView : public CGLView
{
protected: // create from serialization only
	CMeshTestbedView();
	DECLARE_DYNCREATE(CMeshTestbedView)

// Attributes
public:
	CMeshTestbedDoc* GetDocument();
	SolidSTL_Viewer *m_stlViewer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshTestbedView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

	virtual bool SetPartsExtent();	// set the size of parts as m_PartVolMin and m_PartVolMax
	virtual void DrawScene(void);	// overridden to draw STL
	void DrawCoordinate(void);
	void DrawBezierCurv(void);
	void DrawBezierSurf(void);

// Implementation
public:
	virtual ~CMeshTestbedView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static float m_XAxisColor[4];
	static float m_YAxisColor[4];
	static float m_ZAxisColor[4];
	float m_Axis_Length;

// Generated message map functions
protected:
	//{{AFX_MSG(CMeshTestbedView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MeshTestbedView.cpp
inline CMeshTestbedDoc* CMeshTestbedView::GetDocument()
   { return (CMeshTestbedDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHTESTBEDVIEW_H__FE0AF8B0_918D_4C84_B0B5_AE0753417E57__INCLUDED_)
