//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// DemoInfo.cpp : implementation file
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vertex.h"
#include "MeshTestbed.h"
#include "DemoInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoInfo dialog

IMPLEMENT_DYNAMIC(CDemoInfo, CDialogBar)

CDemoInfo::CDemoInfo(CWnd* pParent /*=NULL*/)
	: CDialogBar()
	, bCheck1(true)
	, bCheck2(true)
	, mTime(0)
{
	//{{AFX_DATA_INIT(CDemoInfo)
	mEditVertNum = 0.0;
	mEditTriNum = 0.0;
	mEditHandleNum = 0;
	mEditExtX = 0.0;
	mEditExtY = 0.0;
	mEditExtZ = 0.0;
	//}}AFX_DATA_INIT
}


void CDemoInfo::DoDataExchange(CDataExchange* pDX)
{
	aPdx = pDX;
	CDialogBar::DoDataExchange(pDX);
	BOOL temCheck1 = bCheck1;
	BOOL temCheck2 = bCheck2;

	//{{AFX_DATA_MAP(CDemoInfo)
	DDX_Text(pDX, IDC_EDIT_VERTNUM, mEditVertNum);
	DDX_Text(pDX, IDC_EDIT_TRINUM, mEditTriNum);
	DDX_Text(pDX, IDC_EDIT_HANDLENUM, mEditHandleNum);
	DDX_Text(pDX, IDC_EDIT_EXTX, mEditExtX);
	DDX_Text(pDX, IDC_EDIT_EXTY, mEditExtY);
	DDX_Text(pDX, IDC_EDIT_EXTZ, mEditExtZ);
	DDX_Text(pDX, IDC_EDIT3, mTime);
	DDX_Check(pDX, IDC_CHECK1, temCheck1);
    DDX_Check(pDX, IDC_CHECK2, temCheck2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDemoInfo, CDialogBar)
	//{{AFX_MSG_MAP(CDemoInfo)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoInfo message handlers
BOOL CDemoInfo::InitValues()
{
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	UpdateData(FALSE);
	return TRUE;
}

RadioType CDemoInfo::GetRadioType()
{
	if(IsDlgButtonChecked( IDC_RADIO1 ))	
		return RADIO1;
	else if(IsDlgButtonChecked( IDC_RADIO2 ))	
		return RADIO2;
	else //if(IsDlgButtonChecked( IDC_RADIO3 ))	
		return RADIO3;
}

void CDemoInfo::GetEditVertNumValue(double &VertNumValue)
{
	UpdateData(TRUE);

	VertNumValue = mEditVertNum;
}

void CDemoInfo::GetEditTriNumValue(double &EditTriNumValue)
{
	UpdateData(TRUE);

	EditTriNumValue = mEditTriNum;
}

void CDemoInfo::GetEditHandleNumValue(double &EditHandleNumValue)
{
	UpdateData(TRUE);

	EditHandleNumValue = mEditHandleNum;
}

void CDemoInfo::GetEditExtValues(double *PartExt)
{
	UpdateData(TRUE);

	PartExt[0] = mEditExtX;
	PartExt[1] = mEditExtY;
	PartExt[2] = mEditExtZ;
}

void CDemoInfo::OnBnClickedCheck1()
{
	bCheck1 = !bCheck1;
}

void CDemoInfo::OnBnClickedCheck2()
{
	bCheck2 = !bCheck2;
}

void CDemoInfo::updateTime(double Time) {
	mTime = Time;
	char str[256];
	sprintf(str, "%.2lf", mTime);
	SetDlgItemText(IDC_EDIT3, str);
}