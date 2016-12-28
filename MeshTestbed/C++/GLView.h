//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// GLView.h : interface of the CGLView class
// Create a CView to use OpenGL to render a scene
//////////////////////////////////////////////////////////////////////

#pragma once

// CGLView view

enum PreDefinedView {IsometricView, TopView, BottomView, FrontView, BackView, LeftView, RightView, FreeView};
enum ViewMethod {Shaded, Wireframe, WireframeNoFlat, Points, WireframeShaded, Extent, Curvature};
enum MouseMode {Rotate, Pan, ZoomWindow, Rot_Zoom_Pan, Idle};
// Rot_Zoom_Pan: left button - rotate, mid wheel - zoom, right button - pan

#define IN_MM 25.4
#define TD_PI 3.14159265358979323846
#define TD_INFINIT (1.0e+12)
#define M_SQRT1_2 0.707
#define M_SQRT2 1.414

class CGLView : public CView
{
	DECLARE_DYNCREATE(CGLView)

protected:
	CGLView();           // protected constructor used by dynamic creation
	virtual ~CGLView();

	float m_LightPos[4];
	double m_PartVolMin[3], m_PartVolMax[3];// it need to be set before drawScene()
	double m_ViewVol[6];	// the current view volume in the set view direction
public:
	void ClearFreeViewMatrix();

// Attributes
public:
	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;

	float m_bkcolor[4];
	int m_CurrentView;	// one of PreDefinedView
	int m_ViewMethod;	// one of ViewMethod
	int m_ViewOldMethod;// one of ViewMethod
	int m_MouseMode;	// one of MouseMode

protected:
	// zoom flags
	bool m_DrawZoomFlag;
	bool m_DrawPanFlag;
	bool m_DrawRotateFlag;
	CPoint DownPoint, LastPoint, CurrentPoint;	// mouse positions
	float m_Rotate_Angle;
	float m_Rotate_Axis[3];

	void DrawZoomBox();
	void DrawRotateLine();
	void DrawPanLine();
	void ViewRectZoom(void);
	void ViewPan(bool ifFinal);
	void ViewRotate(bool ifFinal);

private:
	double ViewMatrix[4][4];
	double IViewMatrix[4][4];	// inverse of ViewMatrix

// Operations
public:
	void InitOpenGL();
	void CreateRGBPalette(void);
	BOOL bSetupPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Overrides
	virtual void DrawScene(void) {}		// overridden to draw this view
	virtual bool SetPartsExtent() { return false; }	// call you virtual function to get the size of parts as m_PartVol
	virtual void OnDraw(CDC* pDC);		 // overridden to draw this view

protected:
	void SetupViewingFrustum(void);
	void SetupViewingTransform();
	void GetViewOrientationExtents(float orientationOrigin[3], float TheOtherCorner[3]);
	void FindExtentInView(float MinPt[3], float MaxPt[3], float MinPtInView[3], float MaxPtInView[3]);
	void AdjustView();
	void AdjustLightPos();

	void InvGlRotate(double degree, double x, double y, double z);
	void InvGlTranslate(double x, double y, double z);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CCubeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewFront();
	afx_msg void OnUpdateViewFront(CCmdUI *pCmdUI);
	afx_msg void OnViewTop();
	afx_msg void OnUpdateViewTop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTransparent(CCmdUI *pCmdUI);
//	afx_msg void OnViewRight();
	afx_msg void OnUpdateViewRight(CCmdUI *pCmdUI);
	afx_msg void OnViewShade();
	afx_msg void OnUpdateViewShade(CCmdUI *pCmdUI);
	afx_msg void OnViewWireframe();
	afx_msg void OnUpdateViewWireframe(CCmdUI *pCmdUI);
	afx_msg void OnViewWireframeNoFlat();
	afx_msg void OnUpdateViewWireframeNoFlat(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomall();
	afx_msg void OnUpdateViewZoomall(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomin();
	afx_msg void OnUpdateViewZoomin(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomout(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomwindow();
	afx_msg void OnUpdateViewZoomwindow(CCmdUI *pCmdUI);
	afx_msg void OnViewPoint();
	afx_msg void OnUpdateViewPoint(CCmdUI *pCmdUI);
	afx_msg void OnViewPan();
	afx_msg void OnUpdateViewPan(CCmdUI *pCmdUI);
	afx_msg void OnViewIso();
	afx_msg void OnUpdateViewIso(CCmdUI *pCmdUI);
	afx_msg void OnCameraRotate();
	afx_msg void OnUpdateCameraRotate(CCmdUI *pCmdUI);
	afx_msg void OnViewRight();
	afx_msg void OnViewExtent();
	afx_msg void OnUpdateViewExtent(CCmdUI *pCmdUI);
	afx_msg void OnViewWireframeshaded();
	afx_msg void OnUpdateViewWireframeshaded(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
