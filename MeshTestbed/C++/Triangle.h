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
	CVertex m_Centroid;
	double m_Area;
	double m_Slope;
	int m_ColorID;
	int m_SliceID;
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

	CVertex &Normal() {		return m_Normal;	}
	CVertex	&Centroid() {	return m_Centroid;	}
	int ColorID() { return m_ColorID;  }
	int SliceID() { return m_SliceID;  }

	void SetNormal(CVertex &aN) { m_Normal = aN; }
	void SetCentroid(CVertex &aN) { m_Centroid = aN; }
	void SetSliceID(int sID) { m_SliceID = sID;  }
	void SetColorID(int cID) { m_ColorID = cID;  }

	CVertex ComputeNormal();
	CVertex ComputeCentroid();

	void SetArea(double aA)	{ m_Area = aA;	}
	double Area() {		return m_Area;	}
	double Slope() {	return m_Slope;	}
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
	CVertex m_Centroid;
	int m_SliceID;
	int m_ColorID;
	double m_Area;
	double m_Slope;
	int m_ShellNum;

	int mTag;
public:
	CTriangleID(void);
	CTriangleID(const CTriangleID &rhs);
	CTriangleID(const int p0,const int p1,  const int p2);
	virtual ~CTriangleID(void) {}
	CTriangleID & operator =(const CTriangleID &rhs);
	
	int operator [](int i);
	CVertex &Normal() {	return m_Normal; }
	CVertex &Centroid() {	return m_Centroid; }
	int SliceID() { return m_SliceID;  }
	int ColorID() { return m_ColorID;  }
	double Area() {	return m_Area; }
	int Shell() {	return m_ShellNum; }

	void SetVertex(int Index, int aValue)	{	m_vert[Index] = aValue;	}
	void SetNormal(CVertex &aN) { m_Normal = aN;  };
	void SetCentroid(CVertex &aN) { m_Centroid = aN;  };
	void SetSliceID(int sID) { m_SliceID = sID;  }
	void SetColorID(int cID) {	m_ColorID = cID;	}
	void SetArea(double aA)	{ m_Area = aA;	}
	void SetSlope(double aS)	{ m_Slope = aS;	}
	void SetShellNum(int aShell)	{ m_ShellNum = aShell;	}
	void SetTag(int atag) {	mTag = atag;	}

	int tag() {	return mTag;	}
};
