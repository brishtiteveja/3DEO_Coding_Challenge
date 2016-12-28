//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// GLView.cpp : implementation of the CGLView class
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "MeshTestbed.h"

#include "gl/gl.h"
#include "gl/glu.h"
#include ".\glview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

unsigned char threeto8[8] =
{
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] =
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] =
{
	0, 255
};

static int defaultOverride[13] =
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] =
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

enum Axis {AXIS_X=0, AXIS_Y, AXIS_Z};

// CGLView

IMPLEMENT_DYNCREATE(CGLView, CView)

/////////////////////////////////////////////////////////////////////////////
// CGLView construction/destruction
CGLView::CGLView()
{
	m_pDC = NULL;
	m_pOldPalette = NULL;

	m_LightPos[0] = 0.0f; //AXIS_X
	m_LightPos[1] = 0.0f; //AXIS_Y
	m_LightPos[2] = 50.0f;//AXIS_Z
	m_LightPos[3] = 1.0f;

	m_ViewVol[0] = 0.0f;//left 
	m_ViewVol[1] = 1.0f;//right
	m_ViewVol[2] = 0.0f;//top
	m_ViewVol[3] = 1.0f;//bottom
	m_ViewVol[4] = 0.0f;//near
	m_ViewVol[5] = 1.0f;//far

	m_bkcolor[0] = 0.3f;//0.0f;
	m_bkcolor[1] = 0.3f;//0.25f;
	m_bkcolor[2] = 0.3f;//0.5f;
	m_bkcolor[3] = 0.7f;

	m_PartVolMin[0] = 0.0;
	m_PartVolMin[1] = 0.0;
	m_PartVolMin[2] = 0.0;
	m_PartVolMax[0] = 0.0;
	m_PartVolMax[1] = 0.0;
	m_PartVolMax[2] = 0.0;
	
	m_DrawZoomFlag = false;
	m_DrawPanFlag = false;
	m_DrawRotateFlag = false;
	m_Rotate_Angle = 0.0f;
	m_ViewMethod = Shaded;
	m_MouseMode = Idle;	

	// show isometric view
	m_CurrentView = IsometricView;
	ClearFreeViewMatrix();
	InvGlRotate(-60.0f, 1.0f, 0.0f, 0.0f);
	InvGlRotate(45.0f, 0.0f, 0.0f, 1.0f);
	AdjustView();
	AdjustLightPos();
}

CGLView::~CGLView()
{
}

BEGIN_MESSAGE_MAP(CGLView, CView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FRONT, OnUpdateViewFront)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOP, OnUpdateViewTop)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RIGHT, OnUpdateViewRight)
	ON_COMMAND(ID_VIEW_SHADE, OnViewShade)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHADE, OnUpdateViewShade)
	ON_COMMAND(ID_VIEW_WIREFRAME, OnViewWireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, OnUpdateViewWireframe)
	ON_COMMAND(ID_VIEW_WIREFRAMENOFLAT, OnViewWireframeNoFlat)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAMENOFLAT, OnUpdateViewWireframeNoFlat)
	ON_COMMAND(ID_VIEW_ZOOMALL, OnViewZoomall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMALL, OnUpdateViewZoomall)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_COMMAND(ID_VIEW_ZOOMWINDOW, OnViewZoomwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMWINDOW, OnUpdateViewZoomwindow)
	ON_COMMAND(ID_VIEW_POINT, OnViewPoint)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINT, OnUpdateViewPoint)
	ON_COMMAND(ID_VIEW_PAN, OnViewPan)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateViewPan)
	ON_COMMAND(ID_VIEW_ISO, OnViewIso)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ISO, OnUpdateViewIso)
	ON_COMMAND(ID_CAMERA_ROTATE, OnCameraRotate)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROTATE, OnUpdateCameraRotate)
	ON_COMMAND(ID_VIEW_EXTENT, OnViewExtent)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXTENT, OnUpdateViewExtent)
	ON_COMMAND(ID_VIEW_WIREFRAMESHADED, OnViewWireframeshaded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAMESHADED, OnUpdateViewWireframeshaded)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing
// Set IViewMatrix and ViewMatrix to identities
void CGLView::ClearFreeViewMatrix()
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++) 
		{
			if (i == j) 
			{
				ViewMatrix[i][j] = 1.0;
				IViewMatrix[i][j] = 1.0;
			}
			else 
			{
				ViewMatrix[i][j] = 0.0;
				IViewMatrix[i][j] = 0.0;
			}
		}
	}
}

void CGLView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	glClearColor(m_bkcolor[0], m_bkcolor[1], m_bkcolor[2], m_bkcolor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set the viewport equal to the window size, use the full client area
	glViewport(0, 0, m_oldRect.right, m_oldRect.bottom);

	if(SetPartsExtent()==true) // call you virtual function to get the size of parts as m_PartVol
	{
		// Select the viewing volume
		SetupViewingFrustum();
		// Set up all the transforms pertaining to the current selected view.
		SetupViewingTransform();

		DrawScene();	// call you virtual function
        
		// zoom function
		if (m_DrawZoomFlag) DrawZoomBox();
	}

	glFlush();
	glFinish();
	// For double buffering to take place.
	SwapBuffers(wglGetCurrentDC());
}

// Set Up the viewing volume: consider  Orthographic view only.
// To consider perspectiv view, use the following code:
	//if (m_oldRect.bottom)
	//	fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	//else    // don't divide by zero, not that we should ever run into that...
	//	fAspect = 1.0f;
	//fNearPlane = 3.0f;
	//fFarPlane = 7.0f;
	//fMaxObjSize = 3.0f;
	//m_fRadius = fNearPlane + fMaxObjSize / 2.0f;
	//gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
void CGLView::SetupViewingFrustum(void)
{
	// Select the projection matrix and clear it
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Adjust the computed view-volume, so that it has the same spect-ratio 
	// as the real window. The adjustment may enlarge the view-volume.
	float OrientCorner1[3], OrientCorner2[3];
	GetViewOrientationExtents(OrientCorner1, OrientCorner2);

	float TmpMin[3];
	float TmpMax[3];
	for (int i=0; i<3; i++)
	{
		TmpMin[i] = (float)m_PartVolMin[i];
		TmpMax[i] = (float)m_PartVolMax[i];
	}
	for (int i=0; i<3; i++)
	{
		if (TmpMin[i]>OrientCorner1[i]) TmpMin[i]=OrientCorner1[i];
		if (TmpMin[i]>OrientCorner2[i]) TmpMin[i]=OrientCorner2[i];
		if (TmpMax[i]<OrientCorner1[i]) TmpMax[i]=OrientCorner1[i];
		if (TmpMax[i]<OrientCorner2[i]) TmpMax[i]=OrientCorner2[i];
	}

	float tmpExtentsMin[3];
	float tmpExtentsMax[3];
	float delta;
	for (int i=0; i<3; i++)
	{
		delta = TmpMax[i] - TmpMin[i];
		tmpExtentsMin[i] = TmpMin[i] - 0.5f * delta;
		tmpExtentsMax[i] = TmpMax[i] + 0.5f * delta;
	}

	float ViewMin[3];
	float ViewMax[3];
	FindExtentInView(tmpExtentsMin, tmpExtentsMax, ViewMin, ViewMax);
	//FindExtentInView(TmpMin, TmpMax, ViewMin, ViewMax);
	m_ViewVol[4] =  -ViewMax[AXIS_Z] - 20.0f;
	m_ViewVol[5] =  -ViewMin[AXIS_Z] + 20.0f;

	double VolWidth = m_ViewVol[1]-m_ViewVol[0];
	double VolHeight = m_ViewVol[3]-m_ViewVol[2];
	double WidthRatio = VolWidth / (double) m_oldRect.right;
	double HeightRatio = VolHeight / (double) m_oldRect.bottom;
	double WHFactor;
	double HWFactor;
	double ViewVolume[6];	

	for (int i=0; i<6; i++) 
	{
		ViewVolume[i] = m_ViewVol[i];
	}

	if (WidthRatio > HeightRatio) 
	{
		WHFactor = WidthRatio / HeightRatio;
		ViewVolume[2] = m_ViewVol[2] - (WHFactor-1.0f) * 0.5f* VolHeight;
		ViewVolume[3] = m_ViewVol[3] + (WHFactor-1.0f) * 0.5f* VolHeight;
	}
	else 
	{
		HWFactor = HeightRatio / WidthRatio;
		ViewVolume[0] = m_ViewVol[0] - (HWFactor-1.0f) * 0.5f* VolWidth;
		ViewVolume[1] = m_ViewVol[1] + (HWFactor-1.0f) * 0.5f* VolWidth;
	}
	

	glOrtho(ViewVolume[0], ViewVolume[1], ViewVolume[2], ViewVolume[3], ViewVolume[4], ViewVolume[5]);
}

void CGLView::GetViewOrientationExtents(float orientationOrigin[3], float TheOtherCorner[3])
{
	float m_RatioInX = (float)(m_ViewVol[1] - m_ViewVol[0]) / (float) (m_oldRect.right);
	float m_RatioInY = (float)(m_ViewVol[3] - m_ViewVol[2]) / (float) (m_oldRect.bottom);

	float originOffset = 70.0f;
	float axisLength = 35.0f;
	float originOnScreen[3];
	originOnScreen[AXIS_X] = float(m_ViewVol[0] + originOffset * m_RatioInX);
	originOnScreen[AXIS_Y] = float(m_ViewVol[2] + originOffset * m_RatioInY);
	originOnScreen[AXIS_Z] = 0.0f;

	orientationOrigin[AXIS_X] = float(IViewMatrix[0][0]*originOnScreen[0] +
						   IViewMatrix[0][1]*originOnScreen[1] +
						   IViewMatrix[0][2]*originOnScreen[2] +
						   IViewMatrix[0][3]*1.0f); 

	orientationOrigin[AXIS_Y] = float(IViewMatrix[1][0]*originOnScreen[0] +
						   IViewMatrix[1][1]*originOnScreen[1] +
						   IViewMatrix[1][2]*originOnScreen[2] +
						   IViewMatrix[1][3]*1.0f); 

	orientationOrigin[AXIS_Z] = float(IViewMatrix[2][0]*originOnScreen[0] +
						   IViewMatrix[2][1]*originOnScreen[1] +
						   IViewMatrix[2][2]*originOnScreen[2] +
						   IViewMatrix[2][3]*1.0f);

	TheOtherCorner[AXIS_X] = orientationOrigin[AXIS_X] + axisLength * m_RatioInX;
	TheOtherCorner[AXIS_Y] = orientationOrigin[AXIS_Y] + axisLength * m_RatioInX;
	TheOtherCorner[AXIS_Z] = orientationOrigin[AXIS_Z] + axisLength * m_RatioInX;
}

void CGLView::FindExtentInView(float MinPt[3], float MaxPt[3], 
							   float MinPtInView[3], float MaxPtInView[3])
{
	int i, j;
	float Vertices[8][3];

	Vertices[0][AXIS_X] = MinPt[AXIS_X];
	Vertices[0][AXIS_Y] = MinPt[AXIS_Y];
	Vertices[0][AXIS_Z] = MinPt[AXIS_Z];

	Vertices[1][AXIS_X] = MaxPt[AXIS_X];
	Vertices[1][AXIS_Y] = MinPt[AXIS_Y];
	Vertices[1][AXIS_Z] = MinPt[AXIS_Z];

	Vertices[2][AXIS_X] = MaxPt[AXIS_X];
	Vertices[2][AXIS_Y] = MaxPt[AXIS_Y];
	Vertices[2][AXIS_Z] = MinPt[AXIS_Z];

	Vertices[3][AXIS_X] = MinPt[AXIS_X];
	Vertices[3][AXIS_Y] = MaxPt[AXIS_Y];
	Vertices[3][AXIS_Z] = MinPt[AXIS_Z];
	
	Vertices[4][AXIS_X] = MinPt[AXIS_X];
	Vertices[4][AXIS_Y] = MinPt[AXIS_Y];
	Vertices[4][AXIS_Z] = MaxPt[AXIS_Z];

	Vertices[5][AXIS_X] = MaxPt[AXIS_X];
	Vertices[5][AXIS_Y] = MinPt[AXIS_Y];
	Vertices[5][AXIS_Z] = MaxPt[AXIS_Z];

	Vertices[6][AXIS_X] = MaxPt[AXIS_X];
	Vertices[6][AXIS_Y] = MaxPt[AXIS_Y];
	Vertices[6][AXIS_Z] = MaxPt[AXIS_Z];

	Vertices[7][AXIS_X] = MinPt[AXIS_X];
	Vertices[7][AXIS_Y] = MaxPt[AXIS_Y];
	Vertices[7][AXIS_Z] = MaxPt[AXIS_Z];

	for (i=0; i<3; i++) 
	{
		MaxPtInView[i] = (float) (- TD_INFINIT);
		MinPtInView[i] = (float)TD_INFINIT;
	}

	float Vertex[3];
	for (i=0;i<7;i++) 
	{
		for (j=0; j<3; j++) 
		{
			Vertex[j] = (float) (ViewMatrix[j][0]*Vertices[i][AXIS_X] + 
								 ViewMatrix[j][1]*Vertices[i][AXIS_Y] +
								 ViewMatrix[j][2]*Vertices[i][AXIS_Z] +
								 ViewMatrix[j][3]);
		}

		for (j=0; j<3; j++) 
		{
			if (Vertex[j] > MaxPtInView[j]) MaxPtInView[j] = Vertex[j];
			if (Vertex[j] < MinPtInView[j]) MinPtInView[j] = Vertex[j];
		}
	}

}

//------------------------------------------------------------------
// Do all the transforms pertaining to the current selected view.
// The The ModelView matrix has been cleared to identity in the Draw()
// before this function is called. The translations and rotations for all
// the ViewModes, except the FreeView, have been recorded in the 
// IViewMatrix/ViewMatrix in their callback functions respectively. For
// FreeView mode, the rotations are recorded immediately.
//------------------------------------------------------------------
void CGLView::SetupViewingTransform()
{
	// Switch back to the modelview matrix and clear it to identity.
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	if (m_Rotate_Angle > 0.01 || m_Rotate_Angle < -0.01) // not zero
	{
		float ExtentCenterX = (float)(m_PartVolMin[AXIS_X] + m_PartVolMax[AXIS_X])/2.0f;
		float ExtentCenterY = (float)(m_PartVolMin[AXIS_Y] + m_PartVolMax[AXIS_Y])/2.0f;
		float ExtentCenterZ = (float)(m_PartVolMin[AXIS_Z] + m_PartVolMax[AXIS_Z])/2.0f;

		InvGlTranslate(ExtentCenterX, ExtentCenterY, ExtentCenterZ);
	    InvGlRotate(m_Rotate_Angle, m_Rotate_Axis[0], m_Rotate_Axis[1], m_Rotate_Axis[2]);
		InvGlTranslate(-ExtentCenterX, -ExtentCenterY, -ExtentCenterZ);

		AdjustLightPos(); 
		m_Rotate_Angle = 0.0f;
	}

	double M[16];
	for (int i=0; i<4; i++) 
	{
		for (int j=0; j<4; j++) 
		{
			M[i + j*4] = ViewMatrix[i][j];
		}
	}
	glLoadMatrixd(M); // Use the recorded ViewMatrix
}

//--------------------------------------------------------------------------
// This routine determines the view-volume to contain the combined extent of 
// parts and platform in the view defined by current view-matrix and 
//--------------------------------------------------------------------------
void CGLView::AdjustView()
{
	float fPartMin[3], fPartMax[3];
	for(int i=0;i<3;i++)
	{
		fPartMin[i] = (float)m_PartVolMin[i];
		fPartMax[i] = (float)m_PartVolMax[i];
	}
	float ViewMin[3], ViewMax[3];
	FindExtentInView( fPartMin, fPartMax, ViewMin, ViewMax);

	m_ViewVol[0] =  ViewMin[AXIS_X] - 0.02f*(ViewMax[AXIS_X] - ViewMin[AXIS_X]);//left 
	m_ViewVol[1] =  ViewMax[AXIS_X] + 0.02f*(ViewMax[AXIS_X] - ViewMin[AXIS_X]);//right
	m_ViewVol[2] =  ViewMin[AXIS_Y] - 0.02f*(ViewMax[AXIS_Y] - ViewMin[AXIS_Y]);//top
	m_ViewVol[3] =  ViewMax[AXIS_Y] + 0.02f*(ViewMax[AXIS_Y] - ViewMin[AXIS_Y]);//bottom
	m_ViewVol[4] =  -ViewMax[AXIS_Z] - 1.0f;
	m_ViewVol[5] =  -ViewMin[AXIS_Z] + 1.0f;
}

void CGLView::AdjustLightPos()
{
	float LightX= 40.0f;
	float LightY = 40.0f;
	float LightZ = 50.0f;
	float LightW = 1.0f;
	
	m_LightPos[0] = (GLfloat)(IViewMatrix[0][0]*LightX + IViewMatrix[0][1]*LightY + 
							IViewMatrix[0][2]*LightZ + IViewMatrix[0][3]*LightW);
	m_LightPos[1] = (GLfloat)(IViewMatrix[0][1]*LightX + IViewMatrix[1][1]*LightY + 
							IViewMatrix[1][2]*LightZ + IViewMatrix[1][3]*LightW);
	m_LightPos[2] = (GLfloat)(IViewMatrix[0][2]*LightX + IViewMatrix[2][1]*LightY + 
							IViewMatrix[2][2]*LightZ + IViewMatrix[2][3]*LightW);
	m_LightPos[3] = 1.0f;
}

/////////////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG
void CGLView::AssertValid() const
{
	CView::AssertValid();
}

void CGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style. Refer to SetPixelFormat
	// documentation in the "Comments" section for further information.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitOpenGL(); // initialize OpenGL

	return 0;
}

void CGLView::OnDestroy()
{
	HGLRC   hrc;

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;

	CView::OnDestroy();
}

void CGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_oldRect.right = cx;
	m_oldRect.bottom = cy;
}

/////////////////////////////////////////////////////////////////////////////
// GL helper functions

void CGLView::InitOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	CreateRGBPalette();

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearColor(m_bkcolor[0], m_bkcolor[1], m_bkcolor[2], m_bkcolor[3]);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);
	glDisable(GL_FOG);
}

BOOL CGLView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
//		32,                             // 32-bit z-buffer
  	    16,								// NOTE: better performance with 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}

unsigned char CGLView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char) (i >> shift);
	switch (nbits)
	{

	case 1:
		val &= 0x1;
		return oneto8[val];
	case 2:
		val &= 0x3;
		return twoto8[val];
	case 3:
		val &= 0x7;
		return threeto8[val];

	default:
		return 0;
	}
}

#pragma warning(disable : 4244)
void CGLView::CreateRGBPalette()
{
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int n, i;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		n = 1 << pfd.cColorBits;
		pPal = (PLOGPALETTE) new char[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

		ASSERT(pPal != NULL);

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;
		for (i=0; i<n; i++)
		{
			pPal->palPalEntry[i].peRed =
					ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
			pPal->palPalEntry[i].peGreen =
					ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
			pPal->palPalEntry[i].peBlue =
					ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		/* fix up the palette to include the default GDI palette */
		if ((pfd.cColorBits == 8)                           &&
			(pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
			(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
			(pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
		   )
		{
			for (i = 1 ; i <= 12 ; i++)
				pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
		}

		m_cPalette.CreatePalette(pPal);
		delete [] pPal;

		m_pOldPalette = m_pDC->SelectPalette(&m_cPalette, FALSE);
		m_pDC->RealizePalette();
	}
}
#pragma warning(default : 4244)

BOOL CGLView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;	// do nothing, OpenGL will take care of background
}

// Computate the Rotation Matrix about a given direction and multiply it to the 
// IViewMatrix and ViewMatrix
void CGLView::InvGlRotate(double degree, double x, double y, double z)
{
	int i, j, k;
	double R[4][4];
	double IR[4][4];
	double TMP[4][4];
	double S[3][3];
	double UU[3][3];
	double NUU[3][3];
	double u[3];
	
	// Normalize the direction vector
	double tmp = sqrt(x*x+y*y+z*z);
	u[0] = x / tmp;
	u[1] = y / tmp;
	u[2] = z / tmp;

	S[0][0] = 0.0;
	S[0][1] = -u[2];
	S[0][2] = u[1];

	S[1][0] = u[2];
	S[1][1] = 0.0;
	S[1][2] = -u[0];

	S[2][0] = -u[1];
	S[2][1] = u[0];
	S[2][2] = 0.0;

	for (i=0; i<3; i++) 
	{
		for (j=0; j<3; j++) 
		{
			UU[i][j] = u[i]*u[j];
		}
	}

	for (i=0; i<3; i++) 
	{
		for (j=0; j<3; j++) 
		{
			if (i == j) 
			{
				NUU[i][j] = 1.0 - UU[i][j];
			}
			else 
			{
				NUU[i][j] = -UU[i][j];
			}
		}
	}

	// inverse rotation
	double nsa = sin(-degree*TD_PI/180.0);
	double nca = cos(-degree*TD_PI/180.0);
	for (i=0; i<3; i++) 
	{
		for (j=0; j<3; j++) 
		{
			IR[i][j] = UU[i][j] + nca*NUU[i][j] + nsa*S[i][j];
		}
	}
	IR[0][3] = 0.0;
	IR[1][3] = 0.0;
	IR[2][3] = 0.0;
	IR[3][3] = 1.0;
	IR[3][0] = 0.0;
	IR[3][1] = 0.0;
	IR[3][2] = 0.0;

	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			TMP[i][j] = IViewMatrix[i][j];
		}
	}

	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			tmp = 0.0;
			for (k=0; k<4; k++) 
			{
				tmp += IR[i][k] * TMP[k][j];
			}
			IViewMatrix[i][j] = tmp;
		}
	}

	// rotation matrix
	double sa = sin(degree*TD_PI/180.0);
	double ca = cos(degree*TD_PI/180.0);
	for (i=0; i<3; i++) 
	{
		for (j=0; j<3; j++) 
		{
			R[i][j] = UU[i][j] + ca*NUU[i][j] + sa*S[i][j];
		}
	}
	R[0][3] = 0.0;
	R[1][3] = 0.0;
	R[2][3] = 0.0;
	R[3][3] = 1.0;
	R[3][0] = 0.0;
	R[3][1] = 0.0;
	R[3][2] = 0.0;

	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			TMP[i][j] = ViewMatrix[i][j];
		}
	}

	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			tmp = 0.0;
			for (k=0; k<4; k++)
			{
				tmp += TMP[i][k] * R[k][j];
			}
			ViewMatrix[i][j] = tmp;
		}
	}
}

// Translate the IViewMatrix and ViewMatrix
void CGLView::InvGlTranslate(double x, double y, double z)
{
	int i, j, k;
	double T[4][4];
	double IT[4][4];

	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			if (i == j) 
			{
				T[i][j] = 1.0f;
				IT[i][j] = 1.0f;
			}
			else 
			{
				T[i][j] = 0.0f;
				IT[i][j] = 0.0f;
			}
		}
	}

	IT[0][3] -= x;
	IT[1][3] -= y;
	IT[2][3] -= z;

	T[0][3] += x;
	T[1][3] += y;
	T[2][3] += z;

	double tmp;
	double TMP[4][4];
   	for (i=0; i<4; i++) 
		for (j=0; j<4; j++) 
			TMP[i][j] = IViewMatrix[i][j];
		
	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			tmp = 0.0;
			for (k=0; k<4; k++) 
			{
				tmp += IT[i][k] * TMP[k][j];
			}
			IViewMatrix[i][j] = tmp;
		}
	}

	for (i=0; i<4; i++) 
		for (j=0; j<4; j++) 
			TMP[i][j] = ViewMatrix[i][j];
		
	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			tmp = 0.0;
			for (k=0; k<4; k++) 
			{
				tmp += TMP[i][k] * T[k][j];
			}
			ViewMatrix[i][j] = tmp;
		}
	}
}

// View handling functions
// **********************************************************
// Different view directions
void CGLView::OnViewIso()
{
	m_CurrentView = IsometricView;

	ClearFreeViewMatrix();
	InvGlRotate(-60.0f, 1.0f, 0.0f, 0.0f);
	InvGlRotate(45.0f, 0.0f, 0.0f, 1.0f);
	
	AdjustLightPos();
	AdjustView();
	Invalidate();	
}

void CGLView::OnUpdateViewIso(CCmdUI *pCmdUI)
{
	if (m_CurrentView == IsometricView)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewFront()
{
	m_CurrentView = FrontView;

	ClearFreeViewMatrix();
	InvGlRotate(-90.0f, 1.0f, 0.0f, 0.0f);
	InvGlRotate( 90.0f, 0.0f, 0.0f, 1.0f);

    AdjustLightPos();
	AdjustView();
	Invalidate();	
}

void CGLView::OnUpdateViewFront(CCmdUI *pCmdUI)
{
	if (m_CurrentView == FrontView)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewTop()
{
	m_CurrentView = TopView;

	ClearFreeViewMatrix();
//	InvGlRotate(180.0f, 1.0f, 0.0f, 0.0f);// bottom view

    AdjustLightPos();
	AdjustView();

	Invalidate();	
}

void CGLView::OnUpdateViewTop(CCmdUI *pCmdUI)
{
	if (m_CurrentView == TopView)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewRight()
{
	m_CurrentView = RightView;

	ClearFreeViewMatrix();
	InvGlRotate(-90.0f, 1.0f, 0.0f, 0.0f);
//	InvGlRotate(180.0f, 0.0f, 0.0f, 1.0f);// left view

    AdjustLightPos();
	AdjustView();
	Invalidate();	
}

void CGLView::OnUpdateViewRight(CCmdUI *pCmdUI)
{
	if (m_CurrentView == RightView)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Different view methods
void CGLView::OnViewShade()
{
	m_ViewMethod = Shaded;
	Invalidate();	
}

void CGLView::OnUpdateViewShade(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == Shaded)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewWireframeshaded()
{
	m_ViewMethod = WireframeShaded;
	Invalidate();	
}

void CGLView::OnUpdateViewWireframeshaded(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == WireframeShaded)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewWireframe()
{
	m_ViewMethod = Wireframe;
	Invalidate();	
}

void CGLView::OnUpdateViewWireframe(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == Wireframe)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewWireframeNoFlat()
{
	m_ViewMethod = WireframeNoFlat;
	Invalidate();	
}

void CGLView::OnUpdateViewWireframeNoFlat(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == WireframeNoFlat)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewPoint()
{
	m_ViewMethod = Points;
	Invalidate();	
}

void CGLView::OnUpdateViewPoint(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == Points)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewExtent()
{
	m_ViewMethod = Extent;
	Invalidate();	
}

void CGLView::OnUpdateViewExtent(CCmdUI *pCmdUI)
{
	if (m_ViewMethod == Extent)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}


// Different view positions
void CGLView::OnCameraRotate()
{
	m_MouseMode = Rotate;	
}

void CGLView::OnUpdateCameraRotate(CCmdUI *pCmdUI)
{
	if (m_MouseMode == Rotate)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Compute the X and Y rotation angles from the mouse movements. The rotation
// about the X and Y (parallel to the screen) axes are done in the SetupViewingTransform() 
void CGLView::ViewRotate(bool ifFinal)
{
    // rotate only when mouse moves
	int dx, dy;
//	dx = abs(DownPoint.x - CurrentPoint.x);
//	dy = abs(DownPoint.y - CurrentPoint.y);
	dx = abs(LastPoint.x - CurrentPoint.x);
	dy = abs(LastPoint.y - CurrentPoint.y);
    if ( dx == 0 && dy == 0 ) 
	{
		m_Rotate_Angle =  0.0f;
	}
	else //if(ifMove == false || dx*dx + dy*dy >= 81)	// move for some distance
	{
		float RatioInX = (float)(m_ViewVol[1] - m_ViewVol[0]) / (float) (m_oldRect.right);
		float RatioInY = (float)(m_ViewVol[3] - m_ViewVol[2])  / (float)(m_oldRect.bottom);

		// find the rotation center on the screen corresponding to the origin of the world coordinates
		float eX = (float)(m_PartVolMax[AXIS_X] - m_PartVolMin[AXIS_X]);
		float eY = (float)(m_PartVolMax[AXIS_Y] - m_PartVolMin[AXIS_Y]);
		float eZ = (float)(m_PartVolMax[AXIS_Z] - m_PartVolMin[AXIS_Z]);
		float r = (float)(sqrt(eX*eX + eY*eY + eZ*eZ) / 2.0f);

		float cX = (float)(m_PartVolMax[AXIS_X] + m_PartVolMin[AXIS_X])/2.0f;
		float cY = (float)(m_PartVolMax[AXIS_Y] + m_PartVolMin[AXIS_Y])/2.0f;
		float cZ = (float)(m_PartVolMax[AXIS_Z] + m_PartVolMin[AXIS_Z])/2.0f;
		float center_x = (float) (  ViewMatrix[0][0]*cX +
									ViewMatrix[0][1]*cY +
									ViewMatrix[0][2]*cZ +
									ViewMatrix[0][3]);
		float center_y = (float) (	ViewMatrix[1][0]*cX +
									ViewMatrix[1][1]*cY +
									ViewMatrix[1][2]*cZ +
									ViewMatrix[1][3]);
		int  Rcenter[2]; // rotation center on the screen
		Rcenter[AXIS_X] = int (m_oldRect.right/2.0 - (m_oldRect.right/(m_ViewVol[1] - m_ViewVol[0])) * 
												((m_ViewVol[1] + m_ViewVol[0])/2.0 - center_x) );
		Rcenter[AXIS_Y] = int (m_oldRect.bottom/2.0 + (m_oldRect.bottom/(m_ViewVol[3] - m_ViewVol[2])) * 
												((m_ViewVol[3] + m_ViewVol[2])/2.0 - center_y) );

		// project the FromPoint and ToPoint to the sphere/hyperbola
		float P1[3]; // from point
		float P2[3]; // to point
		//P1[AXIS_X] = RatioInX * (DownPoint.x - Rcenter[AXIS_X]);
		//P1[AXIS_Y] = -RatioInY * (DownPoint.y - Rcenter[AXIS_Y]);
		P1[AXIS_X] = RatioInX * (LastPoint.x - Rcenter[AXIS_X]);
		P1[AXIS_Y] = -RatioInY * (LastPoint.y - Rcenter[AXIS_Y]);
		float d =  (float)sqrt(P1[AXIS_X]*P1[AXIS_X] + P1[AXIS_Y]*P1[AXIS_Y]);
		if (d > 2*r) r = (float)(d / M_SQRT1_2);
		float t, z;
		if (d < r*M_SQRT1_2)    // Inside sphere 
			z = (float)sqrt(r*r - d*d);
		else
		{                       /* On hyperbola */
			t = (float)(r/M_SQRT2);
			z = t*t / d;
		}
		P1[AXIS_Z] = z;

		P2[AXIS_X] = RatioInX * (CurrentPoint.x - Rcenter[AXIS_X]);
		P2[AXIS_Y] = -RatioInY * (CurrentPoint.y - Rcenter[AXIS_Y]);
		d = (float)sqrt(P2[AXIS_X]*P2[AXIS_X] + P2[AXIS_Y]*P2[AXIS_Y]);
		if (d < r*M_SQRT1_2)    /* Inside sphere */
			z = (float)sqrt(r*r - d*d);
		else
		{                       /* On hyperbola */
			t = (float)(r / M_SQRT2);
			z = t*t / d;
		}
		P2[AXIS_Z] = z;
		 // compute the rotation axis
		float P12[3]; // P2-P1
		P12[AXIS_X] = P2[AXIS_X] - P1[AXIS_X];
		P12[AXIS_Y] = P2[AXIS_Y] - P1[AXIS_Y];
		P12[AXIS_Z] = P2[AXIS_Z] - P1[AXIS_Z];
		m_Rotate_Axis[AXIS_X] = float (P1[AXIS_Y]*P12[AXIS_Z] - P12[AXIS_Y]*P1[AXIS_Z]);
		m_Rotate_Axis[AXIS_Y] = float (-P1[AXIS_X]*P12[AXIS_Z] + P12[AXIS_X]*P1[AXIS_Z]);
		m_Rotate_Axis[AXIS_Z] = float (P1[AXIS_X]*P12[AXIS_Y] - P12[AXIS_X]*P1[AXIS_Y]);

 		// express the rotation axis in the rotated coordinates
		float tmp[3];
		tmp[AXIS_X] = m_Rotate_Axis[AXIS_X];
		tmp[AXIS_Y] = m_Rotate_Axis[AXIS_Y];
		tmp[AXIS_Z] = m_Rotate_Axis[AXIS_Z];
		for (int i=0; i<3; i++) 
		{
			m_Rotate_Axis[i] = float (	IViewMatrix[i][AXIS_X]*tmp[AXIS_X] +
										IViewMatrix[i][AXIS_Y]*tmp[AXIS_Y] +
										IViewMatrix[i][AXIS_Z]*tmp[AXIS_Z]);
		}

		// compute the rotation angle
		t = (float)sqrt(P12[AXIS_X]*P12[AXIS_X] + P12[AXIS_Y]*P12[AXIS_Y] + P12[AXIS_Z]*P12[AXIS_Z]) / (2.0f*r);
		if (t > 1.0) t = 1.0;
		if (t < -1.0) t = -1.0;
		m_Rotate_Angle = (float)((2.0f * asin(t) * 180.0f)/ TD_PI);

		Invalidate();
	} 
}

// draw Rotate line
void CGLView::DrawRotateLine()
{
	CPen NewPen(PS_DOT, 1, RGB(0X50, 0X50, 0X50));
	CDC *pDC = GetDC();

	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectObject(&NewPen);

	pDC->MoveTo(DownPoint);
	pDC->LineTo(LastPoint);

	pDC->MoveTo(DownPoint);
	pDC->LineTo(CurrentPoint);
}

void CGLView::OnViewPan()
{
	m_MouseMode = Pan;	
}

void CGLView::OnUpdateViewPan(CCmdUI *pCmdUI)
{
	if (m_MouseMode == Pan)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Translate view volume
void CGLView::ViewPan(bool ifFinal)
{
    // pan only when mouse moves
	if(LastPoint.x == CurrentPoint.x && LastPoint.y == CurrentPoint.y) return;

	float RatioInX = (float)(m_ViewVol[1] - m_ViewVol[0]) / (float) (m_oldRect.right);
	float RatioInY = (float)(m_ViewVol[3] - m_ViewVol[2]) / (float) (m_oldRect.bottom);

	//float deltX = (float)(CurrentPoint.x - DownPoint.x) * RatioInX;
	//float deltY = -(float)(CurrentPoint.y - DownPoint.y)* RatioInY;
	float deltX = (float)(CurrentPoint.x - LastPoint.x) * RatioInX;
	float deltY = -(float)(CurrentPoint.y - LastPoint.y)* RatioInY;

	m_ViewVol[0] -= deltX;
	m_ViewVol[1] -= deltX;
	m_ViewVol[2] -= deltY;
	m_ViewVol[3] -= deltY;

	Invalidate();
}

// draw Pan line
void CGLView::DrawPanLine()
{
	CPen NewPen(PS_DOT, 1, RGB(0X50, 0X50, 0X50));
	CDC *pDC = GetDC();

	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectObject(&NewPen);

	pDC->MoveTo(DownPoint);
	pDC->LineTo(LastPoint);

	pDC->MoveTo(DownPoint);
	pDC->LineTo(CurrentPoint);
}

void CGLView::OnViewZoomall()
{
	AdjustView();
	Invalidate();
}

void CGLView::OnUpdateViewZoomall(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewZoomin()
{
	int ViewZoomPercent = 30;
	float zoomFactor = 1.0f + (float)ViewZoomPercent/100.0f;
	float delt01 = float ((zoomFactor-1.0f)*(m_ViewVol[1]-m_ViewVol[0]));
	float delt23 = float ((zoomFactor-1.0f)*(m_ViewVol[3]-m_ViewVol[2]));

	m_ViewVol[0] += delt01/2.0f;
	m_ViewVol[1] -= delt01/2.0f;
	m_ViewVol[2] += delt23/2.0f;
	m_ViewVol[3] -= delt23/2.0f;

	Invalidate();
}

void CGLView::OnUpdateViewZoomin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewZoomout()
{
	int ViewZoomPercent = 30;
	float zoomFactor = 1.0f - (float)ViewZoomPercent/100.0f;
	float delt01 = float ((zoomFactor-1.0f)*(m_ViewVol[1]-m_ViewVol[0]));
	float delt23 = float ((zoomFactor-1.0f)*(m_ViewVol[3]-m_ViewVol[2]));

	m_ViewVol[0] += delt01/2.0f;
	m_ViewVol[1] -= delt01/2.0f;
	m_ViewVol[2] += delt23/2.0f;
	m_ViewVol[3] -= delt23/2.0f;

	Invalidate();
}

void CGLView::OnUpdateViewZoomout(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
}

void CGLView::OnViewZoomwindow()
{
	m_MouseMode = ZoomWindow;	
}

void CGLView::OnUpdateViewZoomwindow(CCmdUI *pCmdUI)
{
	if (m_MouseMode == ZoomWindow)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// draw Zoom window
void CGLView::DrawZoomBox()
{
	CPen NewPen(PS_DOT, 1, RGB(0X50, 0X50, 0X50));
	CDC *pDC = GetDC();

	pDC->SetROP2(R2_NOTXORPEN);
	pDC->SelectObject(&NewPen);

	CRect rect(DownPoint, LastPoint);
	pDC->Rectangle(rect);

	CRect rect1(DownPoint, CurrentPoint);
	pDC->Rectangle(rect1);
}

// draw window Zoom models
void CGLView::ViewRectZoom(void)
{
	int dX, dY, DX, DY;
	dX = (CurrentPoint.x - DownPoint.x);
	dY = (CurrentPoint.y - DownPoint.y);
	DX = abs(dX);
	DY = abs(dY);

	if (DX>4 && DY>4)//box has to be more than 4x4 pixels, otherwise, do nothing
	{  
		float RatioInX = (float)(m_ViewVol[1] - m_ViewVol[0]) / (float) (m_oldRect.right);
		float RatioInY = (float)(m_ViewVol[3] - m_ViewVol[2]) / (float) (m_oldRect.bottom);
		
		float XzoomFactor, YzoomFactor;
		XzoomFactor = (float) m_oldRect.right / (float) DX;
		YzoomFactor = (float) m_oldRect.bottom / (float) DY;

		double OrgViewVol[6];
		OrgViewVol[0] = m_ViewVol[0];
		OrgViewVol[1] = m_ViewVol[1];
		OrgViewVol[2] = m_ViewVol[2];
		OrgViewVol[3] = m_ViewVol[3];
	
		double delt_V;
		if (XzoomFactor < YzoomFactor) 
		{
			if (dX > 0) 
			{
				m_ViewVol[0] += RatioInX * (DownPoint.x);
				m_ViewVol[1] -= RatioInX * (m_oldRect.right - CurrentPoint.x);
			}
			else 
			{
				m_ViewVol[0] += RatioInX * (CurrentPoint.x);
				m_ViewVol[1] -= RatioInX * (m_oldRect.right - DownPoint.x);
			}	

			m_ViewVol[2] += RatioInY * (float) (m_oldRect.bottom - (CurrentPoint.y + DownPoint.y)) / 2.0f;
			m_ViewVol[3] += RatioInY * (float) (m_oldRect.bottom - (CurrentPoint.y + DownPoint.y)) / 2.0f;
			delt_V = (m_ViewVol[3] - m_ViewVol[2]) * (1.0f - 1.0f/XzoomFactor) / 2.0f;
			m_ViewVol[2] += delt_V;
			m_ViewVol[3] -= delt_V;
		}
		else 
		{
			if (dY > 0) 
			{
				m_ViewVol[3] -= RatioInY * (DownPoint.y);
				m_ViewVol[2] += RatioInY * (m_oldRect.bottom - CurrentPoint.y);
			}
			else 
			{
				m_ViewVol[3] -= RatioInY * (CurrentPoint.y);
				m_ViewVol[2] += RatioInY * (m_oldRect.bottom - DownPoint.y);
			}
            
			m_ViewVol[0] -= RatioInX * (float) (m_oldRect.right - (CurrentPoint.x + DownPoint.x)) / 2.0f;
			m_ViewVol[1] -= RatioInX * (float) (m_oldRect.right - (CurrentPoint.x + DownPoint.x)) / 2.0f;
			delt_V = (m_ViewVol[1] - m_ViewVol[0]) * (1.0f - 1.0f/YzoomFactor) / 2.0f;
			m_ViewVol[0] += delt_V;
			m_ViewVol[1] -= delt_V;
		}

		if ( (m_ViewVol[1] - m_ViewVol[0]) < 0.0001 &&  (m_ViewVol[3] - m_ViewVol[2]) < 0.0001) 
		{
			m_ViewVol[0] = OrgViewVol[0];
			m_ViewVol[1] = OrgViewVol[1];
			m_ViewVol[2] = OrgViewVol[2];
			m_ViewVol[3] = OrgViewVol[3];
		}
	}
}

// Mouse handling functions
void CGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Record the selected mouse position 
	DownPoint.x = point.x;
	DownPoint.y = point.y;
	LastPoint.x = point.x;
	LastPoint.y = point.y;
	CurrentPoint.x = point.x;
    CurrentPoint.y = point.y;
	
	switch(m_MouseMode )
	{
	case Rotate: 
		if(m_DrawRotateFlag == false) m_ViewOldMethod = m_ViewMethod; // in case mouse out of drawing area
		m_ViewMethod = Wireframe;	// set displaying mode for quick displaying
		m_DrawRotateFlag = true;
		m_CurrentView = FreeView;
		break;
	case ZoomWindow: 
		m_DrawZoomFlag = true;
		m_CurrentView = FreeView;
		break;
	case Pan: 
		if(m_DrawPanFlag == false) m_ViewOldMethod = m_ViewMethod; // in case mouse out of drawing area
		m_ViewMethod = Wireframe;	// set displaying mode for quick displaying
		m_DrawPanFlag = true;
		m_CurrentView = FreeView;
		break;
	default:
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}

void CGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( point.x >= 0 && point.x <= m_oldRect.right && 
			point.y >= 0 && point.y <= m_oldRect.bottom ) 
	{
		CurrentPoint.x = point.x;
		CurrentPoint.y = point.y;
	}
	switch(m_MouseMode )
	{
	case Rotate:
		{
			ViewRotate(true);
			m_DrawRotateFlag = false;
			m_ViewMethod = m_ViewOldMethod;
		}
		break;
	case ZoomWindow:
		{
			ViewRectZoom();
			m_DrawZoomFlag = false;
		}
		break;
	case Pan:
		{
			ViewPan(true);
			m_DrawPanFlag = false;
			m_ViewMethod = m_ViewOldMethod;
		}
		break;
	default:
		break;
	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

void CGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( point.x >= 0 && point.x <= m_oldRect.right && 
		point.y >= 0 && point.y <= m_oldRect.bottom ) 
	{
		LastPoint.x = CurrentPoint.x;
    	LastPoint.y = CurrentPoint.y;
    	CurrentPoint.x = point.x;
	  	CurrentPoint.y = point.y;
		switch(m_MouseMode )
		{
		case Rotate:
			if ( m_DrawRotateFlag ) 
				ViewRotate(false);
				//DrawRotateLine();
			break;
		case ZoomWindow:
			if ( m_DrawZoomFlag ) 
				DrawZoomBox();
			break;
		case Pan:
			if( m_DrawPanFlag )
				ViewPan(false);
				//DrawPanLine();
			break;
		default:
			break;
		}
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonDown(nFlags, point);
}

void CGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonUp(nFlags, point);
}
