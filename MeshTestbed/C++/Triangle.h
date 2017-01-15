//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>
#include <vector>
using namespace std;

class CVertex;
class CSolidSTL;

class CTriangle  
{
protected:
	int mTag;
	CVertex m_vert[3];
	CVertex m_Normal;
	double m_Area;
	double m_Slope;
	int m_Color;
public:
	CTriangle(void);
	CTriangle(const CTriangle &rhs);
	CTriangle(const CVertex &p0,const CVertex &p1,  const CVertex &p2);
	CTriangle(	const float x0,const float y0,const float z0,
				const float x1,const float y1,const float z1,
				const float x2,const float y2,const float z2); 
	CTriangle(	const double x0,const double y0,const double z0,
				const double x1,const double y1,const double z1,
				const double x2,const double y2,const double z2); 
	virtual ~CTriangle(void) {}
	CTriangle & operator =(const CTriangle &rhs);
	
	CVertex & operator [](int i);
	void SetTag(int atag) {	mTag = atag;	}
	int tag() {	return mTag;	}

	void SetNormal(CVertex &aN);
	CVertex &Normal() {		return m_Normal;	}
	CVertex ComputeNormal();

	void SetArea(double aA)	{ m_Area = aA;	}
	double Area() {		return m_Area;	}
	double CalcArea();

	double CalcSlope();

	CTriangle GetReverseTri(); // same postion with reverse normal
	int ReadTriangle(CSolidSTL *pStl, int iIndex);

	static int Generate_BinarySTL(vector<CTriangle> &TriArray, CString &strFileName);
	static int Generate_BinarySTL(int TriNum, CTriangle *TriArray, CString &strFileName);
	static int Combine_BinarySTL(int TriNum, CTriangle *TriArray, CString &strFileName);
	static int Combine_BinarySTL(vector<CTriangle> &TriArray, CString &strFileName);
	static int Read_BinarySTLFile(CString &STLFileName, CTriangle *&pArray_Tri);
};

class CTriangleID  
{
protected:
	int m_vert[3];
	CVertex m_Normal;
	double m_Area;
	int m_ShellNum;

	int mTag;
	int mColorID;
public:
	CTriangleID(void);
	CTriangleID(const CTriangleID &rhs);
	CTriangleID(const int p0,const int p1,  const int p2);
	virtual ~CTriangleID(void) {}
	CTriangleID & operator =(const CTriangleID &rhs);
	
	int operator [](int i);
	CVertex Normal() {	return m_Normal; }
	double Area() {	return m_Area; }
	int Shell() {	return m_ShellNum; }

	void SetVertex(int Index, int aValue)	{	m_vert[Index] = aValue;	}
	void SetNormal(CVertex &aN);
	void SetArea(double aA)	{ m_Area = aA;	}
	void SetShellNum(int aShell)	{ m_ShellNum = aShell;	}
	void SetTag(int atag) {	mTag = atag;	}
	void SetColorID(int aID) {	mColorID = aID;	}

	int tag() {	return mTag;	}
	int colorID() {	return mColorID;	}
};
