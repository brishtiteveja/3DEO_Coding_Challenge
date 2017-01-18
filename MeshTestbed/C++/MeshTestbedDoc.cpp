//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MeshTestbedDoc.cpp : implementation of the CMeshTestbedDoc class
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshTestbed.h"

#include "DemoInfo.h"
#include "MainFrm.h"
#include "MeshTestbedView.h"
#include "MeshTestbedDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedDoc

IMPLEMENT_DYNCREATE(CMeshTestbedDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshTestbedDoc, CDocument)
	//{{AFX_MSG_MAP(CMeshTestbedDoc)
	ON_COMMAND(ID_FILE_OPENSTL1, OnFileOpenSTL1)
	ON_COMMAND(ID_FILE_OPENSTL2, OnFileOpenSTL2)
	ON_COMMAND(ID_FILE_OPENSTL3, OnFileOpenSTL3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedDoc construction/destruction

CMeshTestbedDoc::CMeshTestbedDoc()
{
	m_PartNum = 0;
	for(int i=0; i<MAXIMUM_PARTS; i++)
		mPart[i] = NULL;

	stepSize = 0.01;
	BezierVNum = 100; //1.0/stepSize
	BezierCurv = new CVertex[BezierVNum+1];

	BezierSurfVNumX = 4*5;
	BezierSurfVNumY = 4*5;
	BezierSurfVNum =  (BezierSurfVNumX+1)*(BezierSurfVNumY+1);
	BezierSurf = new CVertex[BezierSurfVNum];
}

CMeshTestbedDoc::~CMeshTestbedDoc()
{
	for(int i=0; i<MAXIMUM_PARTS; i++)
	{
		if(mPart[i] != NULL) delete mPart[i];
		mPart[i] = NULL;
	}
	if(BezierCurv != NULL) delete []BezierCurv;
	if(BezierSurf != NULL) delete []BezierSurf;
}

CMeshTestbedView *CMeshTestbedDoc::GetView(void)
{
	POSITION pos = GetFirstViewPosition();
	CMeshTestbedView* pCurrentView = (CMeshTestbedView*)GetNextView(pos);
	return pCurrentView;
}

BOOL CMeshTestbedDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedDoc serialization

void CMeshTestbedDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedDoc diagnostics

#ifdef _DEBUG
void CMeshTestbedDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMeshTestbedDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedDoc commands
void CMeshTestbedDoc::OnFileOpenSTL1()
{
	OpenSTL(0);
	if(m_PartNum < 1) m_PartNum = 1;
}

void CMeshTestbedDoc::OnFileOpenSTL2()
{
	OpenSTL(1);
	if(m_PartNum < 2) m_PartNum = 2;
}

void CMeshTestbedDoc::OnFileOpenSTL3()
{
	OpenSTL(2);
	if(m_PartNum < 3) m_PartNum = 3;
}

void CMeshTestbedDoc::OpenSTL(int aPartNum)
{
	if(aPartNum <0 || aPartNum > MAXIMUM_PARTS) return;

	CMeshTestbedView* pCurrentView = GetView();
	if(pCurrentView == NULL) return;

	DWORD flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	char szFilter[ ] = "Input STL (*.stl)|*.stl||";
	CString InputSTLFileName;

	CFileDialog Fdlg( TRUE, "*.stl", NULL, flags, szFilter );
	Fdlg.m_ofn.lpstrTitle = "Input STL File";
	INT_PTR response = Fdlg.DoModal();
	if (response == IDOK)
	{
		pCurrentView->BeginWaitCursor();
		CMainFrame *pFrame = (CMainFrame*) pCurrentView->GetParentFrame();
		CProgressCtrl *wndProgressCtrl = pFrame->CreateProgressBar();
		pFrame->m_wndProgressCtrl->SetPos(0);

		InputSTLFileName =  Fdlg.GetPathName();	
		if(mPart[aPartNum] != NULL) delete mPart[aPartNum];
		mPart[aPartNum] = new SolidSTL(InputSTLFileName);
		mPart[aPartNum]->Build(false, pFrame->m_wndProgressCtrl, pFrame->m_wndDemoInfoBar);

		pFrame->m_wndDemoInfoBar->UpdateData(FALSE);

		pFrame->m_wndProgressCtrl->SetPos(100);
		pFrame->DestroyProgressBar();
		pCurrentView->EndWaitCursor();	
		pCurrentView->Invalidate();
	}
}

void CMeshTestbedDoc::GetBoundingBox(double *PartsMin, double *PartsMax)
{
	double PartsVolMin[3], PartsVolMax[3];
	PartsVolMin[0] = PartsVolMin[1] = PartsVolMin[2] = 1e10;
	PartsVolMax[0] = PartsVolMax[1] = PartsVolMax[2] = -1e10;

	for(int i=0; i<m_PartNum; i++)
	{
		if(mPart[i] != NULL)
		{
			double PartVolMin[3], PartVolMax[3];
			mPart[i]->GetBoundingBox(PartVolMin, PartVolMax);
			for(int kk=0; kk<3; kk++)
			{
				if(PartVolMin[kk] < PartsVolMin[kk]) PartsVolMin[kk] = PartVolMin[kk];
				if(PartVolMax[kk] > PartsVolMax[kk]) PartsVolMax[kk] = PartVolMax[kk];
			}
		}
	}

	for(int i=0;i<3;i++)
	{
		PartsMin[i] = PartsVolMin[i];
		PartsMax[i] = PartsVolMax[i];
	}
}
