//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// MeshTestbedView.cpp : implementation of the CMeshTestbedView class
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "MeshTestbed.h"

#include "gl/gl.h"
#include "gl/glu.h"

#include "Vertex.h"
#include "Triangle.h"
#include "SolidSTL.h"
#include "SolidSTL_Viewer.h"

#include "MeshTestbedDoc.h"
#include "MeshTestbedView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedView

IMPLEMENT_DYNCREATE(CMeshTestbedView, CGLView)

BEGIN_MESSAGE_MAP(CMeshTestbedView, CGLView)
	//{{AFX_MSG_MAP(CMeshTestbedView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedView construction/destruction

CMeshTestbedView::CMeshTestbedView()
{
	m_stlViewer = new SolidSTL_Viewer;
	m_Axis_Length = 5.0f;
}

CMeshTestbedView::~CMeshTestbedView()
{
	if(m_stlViewer != NULL) delete m_stlViewer;
}

BOOL CMeshTestbedView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedView drawing

void CMeshTestbedView::OnDraw(CDC* pDC)
{
	CMeshTestbedDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CGLView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedView diagnostics

#ifdef _DEBUG
void CMeshTestbedView::AssertValid() const
{
	CGLView::AssertValid();
}

void CMeshTestbedView::Dump(CDumpContext& dc) const
{
	CGLView::Dump(dc);
}

CMeshTestbedDoc* CMeshTestbedView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshTestbedDoc)));
	return (CMeshTestbedDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshTestbedView message handlers
bool CMeshTestbedView::SetPartsExtent()	// set the size of parts as m_PartVolMin and m_PartVolMax
{
	CMeshTestbedDoc *pdoc = GetDocument();
	
	double PartsVolMin[3], PartsVolMax[3];
	PartsVolMin[0] = PartsVolMin[1] = PartsVolMin[2] = 1e10;
	PartsVolMax[0] = PartsVolMax[1] = PartsVolMax[2] = -1e10;

	if(pdoc->m_PartNum > 0) 
		pdoc->GetBoundingBox(PartsVolMin, PartsVolMax);

	// need to judge the size of coordinate axis
	for(int kk=0; kk<3; kk++)
	{
		if(0.0 < PartsVolMin[kk]) PartsVolMin[kk] = 0.0;
		if(m_Axis_Length > PartsVolMax[kk]) PartsVolMax[kk] = m_Axis_Length;
	}

	if( fabs(m_PartVolMin[AXIS_X]-PartsVolMin[AXIS_X]) > 0.0001 ||
		fabs(m_PartVolMin[AXIS_Y]-PartsVolMin[AXIS_Y]) > 0.0001 ||
		fabs(m_PartVolMin[AXIS_Z]-PartsVolMin[AXIS_Z]) > 0.0001 ||
		fabs(m_PartVolMax[AXIS_X]-PartsVolMax[AXIS_X]) > 0.0001 ||
		fabs(m_PartVolMax[AXIS_Y]-PartsVolMax[AXIS_Y]) > 0.0001 ||
		fabs(m_PartVolMax[AXIS_Z]-PartsVolMax[AXIS_Z]) > 0.0001 )
	{
		m_PartVolMin[AXIS_X]=PartsVolMin[AXIS_X];
		m_PartVolMin[AXIS_Y]=PartsVolMin[AXIS_Y];
		m_PartVolMin[AXIS_Z]=PartsVolMin[AXIS_Z];
		m_PartVolMax[AXIS_X]=PartsVolMax[AXIS_X];
		m_PartVolMax[AXIS_Y]=PartsVolMax[AXIS_Y];
		m_PartVolMax[AXIS_Z]=PartsVolMax[AXIS_Z];
		AdjustView();	// recalculate m_ViewVol for the changed part size
	}
	
	return true;
}

void CMeshTestbedView::DrawScene(void)
{
	CMeshTestbedDoc *pdoc = GetDocument();
	int ViewSkipVert = 2;

	DrawCoordinate();	// added for HW2
	DrawBezierCurv();	// added for HW4
	DrawBezierSurf();	// added for HW4

	BeginWaitCursor();
	for(int i=0; i<pdoc->m_PartNum; i++)
	{
		if(pdoc->mPart[i] != NULL)
		{
			double elapsed_time;
			clock_t start;
			char str[256];

			switch(m_ViewMethod)
			{
			case Points:
				m_stlViewer->DrawModel_Points(pdoc->mPart[i], ViewSkipVert ); // display only one point every 2 points
				break;
			case Shaded:
				m_stlViewer->DrawModel_Shaded( pdoc->mPart[i], m_LightPos );
				break;
			case WireframeShaded:
				m_stlViewer->DrawModel_WireFrameShaded( pdoc->mPart[i], m_LightPos );
				break;
			case Wireframe:
				m_stlViewer->DrawModel_WireFrame( pdoc->mPart[i] );
				break;
			case WireframeNoFlat:
				m_stlViewer->DrawModel_WireFrameNoFlat( pdoc->mPart[i] );
				break;
			case Extent:
				m_stlViewer->DrawModel_Extent( pdoc->mPart[i] );
				break;
			case ColorSTL:
				start = clock();
				m_stlViewer->DrawModel_ColorSTL( pdoc->mPart[i], m_LightPos );
				elapsed_time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000.0);

				pdoc->mPart[i]->aDemoInfo->updateTime(elapsed_time);
				break;
			default:
				// do nothing
				break;
			}
		}
	}
	EndWaitCursor();
}

float CMeshTestbedView::m_XAxisColor[4] = {1.0f, 0.0f, 0.0f, 1.0f };
float CMeshTestbedView::m_YAxisColor[4] = {0.0f, 1.0f, 0.0f, 1.0f };
float CMeshTestbedView::m_ZAxisColor[4] = {0.0f, 0.0f, 1.0f, 1.0f };

void CMeshTestbedView::DrawCoordinate(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPointSize(4.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	// X AXIS
	glColor4fv(m_XAxisColor);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(m_Axis_Length, 0.0, 0.0);
	glEnd();

	// Y AXIS
	glColor4fv(m_YAxisColor);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, m_Axis_Length, 0.0);
	glEnd();

	// Z AXIS
	glColor4fv(m_ZAxisColor);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0,m_Axis_Length);
	glEnd();
}

void CMeshTestbedView::DrawBezierCurv(void)
{
	CMeshTestbedDoc *pdoc = GetDocument();
	if(pdoc == NULL) return;

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	// draw control vertex
	glPointSize(6.0f);
	for(int i=0; i<4; i++)
	{
		glBegin(GL_POINTS);
			glVertex3f((float)pdoc->Bezier_CtrlV[i][0], 
				(float)pdoc->Bezier_CtrlV[i][1], (float)pdoc->Bezier_CtrlV[i][2]);
		glEnd();
	}

	// draw control polygon
	glColor4fv(m_XAxisColor);
	glLineWidth(4.0);
	for(int i=0; i<3; i++)
	{
		glBegin(GL_LINES);
			glVertex3f((float)pdoc->Bezier_CtrlV[i][0], 
				(float)pdoc->Bezier_CtrlV[i][1], (float)pdoc->Bezier_CtrlV[i][2]);
			glVertex3f((float)pdoc->Bezier_CtrlV[i+1][0], 
				(float)pdoc->Bezier_CtrlV[i+1][1], (float)pdoc->Bezier_CtrlV[i+1][2]);
		glEnd();
	}

	// draw curve
	glColor4fv(m_ZAxisColor);
	glLineWidth(2.0);
	for(int i=0; i<pdoc->BezierVNum; i++)
	{
		glBegin(GL_LINES);
			glVertex3f((float)pdoc->BezierCurv[i][0], 
				(float)pdoc->BezierCurv[i][1],(float) pdoc->BezierCurv[i][2]);
			glVertex3f((float)pdoc->BezierCurv[i+1][0], 
				(float)pdoc->BezierCurv[i+1][1], (float)pdoc->BezierCurv[i+1][2]);
		glEnd();
	}
}

void CMeshTestbedView::DrawBezierSurf(void)
{
	CMeshTestbedDoc *pdoc = GetDocument();
	if(pdoc == NULL) return;

	glDisable(GL_CULL_FACE);
	// Set the light model
	//glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_FILL) ; 
	glPolygonMode (GL_BACK, GL_FILL) ; 

	// draw control vertex
	glPointSize(6.0f);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
		{
			glBegin(GL_POINTS);
				glVertex3f((float)pdoc->BezierSurf_CtrlV[i][j][0], 
					(float)pdoc->BezierSurf_CtrlV[i][j][1], (float)pdoc->BezierSurf_CtrlV[i][j][2]);
			glEnd();
		}

	// draw control polygon
	glColor4fv(m_XAxisColor);
	glLineWidth(4.0);
	for(int i=0; i<4; i++)
		for(int j=0; j<3; j++)
		{
			glBegin(GL_LINES);
				glVertex3f((float)pdoc->BezierSurf_CtrlV[i][j][0], 
					(float)pdoc->BezierSurf_CtrlV[i][j][1], (float)pdoc->BezierSurf_CtrlV[i][j][2]);
				glVertex3f((float)pdoc->BezierSurf_CtrlV[i][j+1][0], 
					(float)pdoc->BezierSurf_CtrlV[i][j+1][1], (float)pdoc->BezierSurf_CtrlV[i][j+1][2]);

				glVertex3f((float)pdoc->BezierSurf_CtrlV[j][i][0], 
					(float)pdoc->BezierSurf_CtrlV[j][i][1], (float)pdoc->BezierSurf_CtrlV[j][i][2]);
				glVertex3f((float)pdoc->BezierSurf_CtrlV[j+1][i][0], 
					(float)pdoc->BezierSurf_CtrlV[j+1][i][1], (float)pdoc->BezierSurf_CtrlV[j+1][i][2]);
			glEnd();
		}

	// draw surface (triangles)
	glLineWidth(2.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_ZAxisColor);
	for(int i=0; i<pdoc->BezierSurfVNumY; i++)
		for(int j=0; j<pdoc->BezierSurfVNumX; j++)
		{
			CTriangle T1(pdoc->BezierSurf[i*(pdoc->BezierSurfVNumX+1)+j],
				pdoc->BezierSurf[i*(pdoc->BezierSurfVNumX+1)+j+1], 
				pdoc->BezierSurf[(i+1)*(pdoc->BezierSurfVNumX+1)+j]);
			CTriangle T2(pdoc->BezierSurf[i*(pdoc->BezierSurfVNumX+1)+j+1],
				pdoc->BezierSurf[(i+1)*(pdoc->BezierSurfVNumX+1)+j+1], 
				pdoc->BezierSurf[(i+1)*(pdoc->BezierSurfVNumX+1)+j]);

			glBegin(GL_TRIANGLES);
			// display method for FLAT shading 
			glNormal3f((float)T1.Normal()[AXIS_X], (float)T1.Normal()[AXIS_Y], (float)T1.Normal()[AXIS_Z]);
			glVertex3f((float)T1[0][AXIS_X], (float)T1[0][AXIS_Y], (float)T1[0][AXIS_Z]);
			glVertex3f((float)T1[1][AXIS_X], (float)T1[1][AXIS_Y], (float)T1[1][AXIS_Z]);
			glVertex3f((float)T1[2][AXIS_X], (float)T1[2][AXIS_Y], (float)T1[2][AXIS_Z]);

			glNormal3f((float)T2.Normal()[AXIS_X], (float)T2.Normal()[AXIS_Y], (float)T2.Normal()[AXIS_Z]);
			glVertex3f((float)T2[0][AXIS_X], (float)T2[0][AXIS_Y], (float)T2[0][AXIS_Z]);
			glVertex3f((float)T2[1][AXIS_X], (float)T2[1][AXIS_Y], (float)T2[1][AXIS_Z]);
			glVertex3f((float)T2[2][AXIS_X], (float)T2[2][AXIS_Y], (float)T2[2][AXIS_Z]);
			glEnd();
		}
}
