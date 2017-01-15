//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <math.h>
#include <assert.h>
#include <float.h>
#include <vector>
using namespace std;

#include ".\vertex.h"
#include ".\triangle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangle::CTriangle(void)
{	
	mTag = 0;
}

CTriangle::CTriangle(const CTriangle &rhs)
{
	mTag = rhs.mTag;
	m_vert[0] = rhs.m_vert[0];
	m_vert[1] = rhs.m_vert[1];
	m_vert[2] = rhs.m_vert[2];
	m_Normal = rhs.m_Normal;
	m_Area = rhs.m_Area;
	m_Slope = rhs.m_Slope;
}

CTriangle::CTriangle(const CVertex &p0, const CVertex &p1, const CVertex &p2)
{
	m_vert[0] = p0;
	m_vert[1] = p1;
	m_vert[2] = p2;
	ComputeNormal();
	CalcArea();
	CalcSlope();
}      

CTriangle::CTriangle(const float x0, const float y0, const float z0,
					 const float x1, const float y1, const float z1,
					 const float x2, const float y2, const float z2) 
{
	m_vert[0][0] = x0;	m_vert[0][1] = y0;	m_vert[0][2] = z0;
	m_vert[1][0] = x1;	m_vert[1][1] = y1;	m_vert[1][2] = z1;
	m_vert[2][0] = x2;	m_vert[2][1] = y2;	m_vert[2][2] = z2;
	ComputeNormal();
	CalcArea();
	CalcSlope();
}
	
CTriangle::CTriangle(const double x0, const double y0, const double z0,
					 const double x1, const double y1, const double z1,
					 const double x2, const double y2, const double z2) 
{
	m_vert[0][0] = x0;	m_vert[0][1] = y0;	m_vert[0][2] = z0;
	m_vert[1][0] = x1;	m_vert[1][1] = y1;	m_vert[1][2] = z1;
	m_vert[2][0] = x2;	m_vert[2][1] = y2;	m_vert[2][2] = z2;
	ComputeNormal();
	CalcArea();
	CalcSlope();
}

CTriangle& CTriangle::operator =(const CTriangle &rhs)
{
	mTag = rhs.mTag;
	m_vert[0][0] = rhs.m_vert[0][0];
	m_vert[0][1] = rhs.m_vert[0][1];
	m_vert[0][2] = rhs.m_vert[0][2];
	m_vert[1][0] = rhs.m_vert[1][0];
	m_vert[1][1] = rhs.m_vert[1][1];
	m_vert[1][2] = rhs.m_vert[1][2];
	m_vert[2][0] = rhs.m_vert[2][0];
	m_vert[2][1] = rhs.m_vert[2][1];
	m_vert[2][2] = rhs.m_vert[2][2];
	m_Normal = rhs.m_Normal;
	m_Area = rhs.m_Area;
	return *this;
}

CVertex& CTriangle::operator [](int i)
{  
	return m_vert[i];
}

int CTriangle::ReadTriangle(CSolidSTL *pstl, int iIndex)
{
	//if (iIndex < pstl->num_Triangles())
	//{
	//	m_vert[0] = pstl->nth_Vert(pstl->nth_Tri(iIndex)[0]);
	//	m_vert[1] = pstl->nth_Vert(pstl->nth_Tri(iIndex)[1]);
	//	m_vert[2] = pstl->nth_Vert(pstl->nth_Tri(iIndex)[2]);

	//	return 0;
	//}
	return -1;
}

void CTriangle::SetNormal(CVertex &aN)
{
	m_Normal = aN;
}

CVertex CTriangle::ComputeNormal()
{
	CVertex p1, p2, f;
	double m;				// magnitude of normal

	// compute cross product of v1->v2 & v1->v3
	// compute vector components
	p1 = m_vert[1] - m_vert[0];
	p2 = m_vert[2] - m_vert[0];

	// compute the cross product p1xp2
	f[AXIS_X] = (p1[AXIS_Y] * p2[AXIS_Z]) - (p1[AXIS_Z] * p2[AXIS_Y]);
	f[AXIS_Y] = (p1[AXIS_Z] * p2[AXIS_X]) - (p1[AXIS_X] * p2[AXIS_Z]);
	f[AXIS_Z] = (p1[AXIS_X] * p2[AXIS_Y]) - (p1[AXIS_Y] * p2[AXIS_X]);

	// compute the magnitude
	m = sqrt((f[AXIS_X] * f[AXIS_X]) + (f[AXIS_Y] * f[AXIS_Y]) + (f[AXIS_Z] * f[AXIS_Z]));
	if (m < 1.0e-10)
	{
		f[AXIS_X] = 0.0f;
		f[AXIS_Y] = 0.0f;
		f[AXIS_Z] = 0.0f;
	}
	else
	{
		// normalize the return values
		f /= m;
	}
	m_Normal = f;
	return f;
}

double CTriangle::CalcArea()
{
	CVertex &v1= m_vert[0];
	CVertex &v2= m_vert[1];
	CVertex &v3= m_vert[2];

	double v1v2 = (v1[AXIS_X] - v2[AXIS_X])*(v1[AXIS_X] - v2[AXIS_X]) +
				  (v1[AXIS_Y] - v2[AXIS_Y])*(v1[AXIS_Y] - v2[AXIS_Y]) +
				  (v1[AXIS_Z] - v2[AXIS_Z])*(v1[AXIS_Z] - v2[AXIS_Z]);
	double v1v3 = (v1[AXIS_X] - v3[AXIS_X])*(v1[AXIS_X] - v3[AXIS_X]) +
				  (v1[AXIS_Y] - v3[AXIS_Y])*(v1[AXIS_Y] - v3[AXIS_Y]) +
				  (v1[AXIS_Z] - v3[AXIS_Z])*(v1[AXIS_Z] - v3[AXIS_Z]);
	double v2v3 = (v2[AXIS_X] - v3[AXIS_X])*(v2[AXIS_X] - v3[AXIS_X]) +
				  (v2[AXIS_Y] - v3[AXIS_Y])*(v2[AXIS_Y] - v3[AXIS_Y]) +
				  (v2[AXIS_Z] - v3[AXIS_Z])*(v2[AXIS_Z] - v3[AXIS_Z]);

	double x1,y1,z1,x2,y2,z2,x3,y3,z3,l3,l1;
    if (v1v2 >= v1v3 && v1v2 >= v2v3)
    {
		x1 = v1[AXIS_X];  y1 = v1[AXIS_Y];  z1 = v1[AXIS_Z];
		x2 = v2[AXIS_X];  y2 = v2[AXIS_Y];  z2 = v2[AXIS_Z];
		x3 = v3[AXIS_X];  y3 = v3[AXIS_Y];  z3 = v3[AXIS_Z];
		l1 = v1v2;   l3 = v2v3;
    }
	else if (v1v3 >= v1v2 && v1v3 >= v2v3)
    {
		x1 = v1[AXIS_X];  y1 = v1[AXIS_Y];   z1 = v1[AXIS_Z];
		x2 = v3[AXIS_X];  y2 = v3[AXIS_Y];   z2 = v3[AXIS_Z];
		x3 = v2[AXIS_X];  y3 = v2[AXIS_Y];   z3 = v2[AXIS_Z];
		l1 = v1v3;   l3 = v2v3;
    }
	else 
    {
		x1 = v2[AXIS_X];  y1 = v2[AXIS_Y];  z1 = v2[AXIS_Z];
		x2 = v3[AXIS_X];  y2 = v3[AXIS_Y];  z2 = v3[AXIS_Z];
		x3 = v1[AXIS_X];  y3 = v1[AXIS_Y];  z3 = v1[AXIS_Z];
		l1 = v2v3;   l3 = v1v3;
    }
	double t1 = x2*(x2-x1-x3) + y2*(y2 -y1 -y3) + z2*(z2 -z1-z3) +x1*x3 +y1*y3 +z1*z3;
	t1 *= t1;
	double tem = l1 * ( l3 - t1/(x1*(x1 - 2.0*x2) + x2*x2 + 
								 y1*(y1 - 2.0*y2) + y2*y2 + 
								 z1*(z1 - 2.0*z2) + z2*z2));
	if(tem > 1.0e-12)
		m_Area = sqrt(tem)/2.0;
	else 
		m_Area = 0.0;

	//CVertex p1, p2, f;
	//double m, ms;				// magnitude of normal

	//// compute cross product of v1->v2 & v1->v3
	//// compute vector components
	//p1 = m_vert[1] - m_vert[0];
	//p2 = m_vert[2] - m_vert[0];

	//// compute the cross product p1xp2
	//f[AXIS_X] = (p1[AXIS_Y] * p2[AXIS_Z]) - (p1[AXIS_Z] * p2[AXIS_Y]);
	//f[AXIS_Y] = (p1[AXIS_Z] * p2[AXIS_X]) - (p1[AXIS_X] * p2[AXIS_Z]);
	//f[AXIS_Z] = (p1[AXIS_X] * p2[AXIS_Y]) - (p1[AXIS_Y] * p2[AXIS_X]);

	//// compute the magnitude
	//m = (f[AXIS_X] * f[AXIS_X]) + (f[AXIS_Y] * f[AXIS_Y]) + (f[AXIS_Z] * f[AXIS_Z]);	
	//if (m < 1.0e-6) m_Area = 0.0;
	//else
	//{
	//	ms = sqrt(m);
	//	m_Area = ms;
	//}
	return m_Area;
}

double CTriangle::CalcSlope()
{
	// Getting reference surface normal vector
	CVertex m_Ref_Surface_Norm = CVertex(0.0, 0.0, 1.0);
	// Get angle between m_Norm and m_Ref_Surface_Norm
	double m_Norm_Dot_m_Ref = m_Normal.dot(m_Ref_Surface_Norm);
	double angle = acos(m_Norm_Dot_m_Ref);

	if (abs(angle - M_PI / 2.0) < 1e-06) {
		printf("slope = %lf\n", INFINITY);
		return INFINITY;
	}
	else {
		double slope = tan(angle);
		printf("slope = %lf\n", slope);
		return slope;
	}
}
    
CTriangle CTriangle::GetReverseTri()
{
	CTriangle rTri(m_vert[0], m_vert[2], m_vert[1]);
	return rTri;
}

int CTriangle::Generate_BinarySTL(vector<CTriangle> &TriArray, 
									 CString &strFileName)
{
	char header[80] = "3DSystems Generated by YC ";

	LPTSTR lpszSystemInfo;     // pointer to system information string 
	DWORD cchBuff = 256;       // size of computer or user name 
	TCHAR tchBuffer2[512];
 	lpszSystemInfo = tchBuffer2; 
 
	// Get and display the name of the computer. 
	GetComputerName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);
	strcat(header," ");
	GetUserName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);

	FILE *fptr = NULL;
	fptr = fopen(strFileName, "wb");
	if(fptr == NULL) return -1;

	// write the header
	fwrite(header, 1, 80, fptr);
	// write tri number
	int num_tri = (int)TriArray.size();
	fwrite((char *)&num_tri, sizeof(int), 1, fptr);

	// write each triangle
	for(int i=0;i<num_tri;i++)
	{
		CTriangle& curTri = TriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}

	fclose(fptr);
	return num_tri;
}

int CTriangle::Generate_BinarySTL(int TriNum, CTriangle *TriArray, CString &strFileName)
{
	char header[80] = "3DSystems Generated by YC ";

	LPTSTR lpszSystemInfo;     // pointer to system information string 
	DWORD cchBuff = 256;       // size of computer or user name 
	TCHAR tchBuffer2[512];
 	lpszSystemInfo = tchBuffer2; 
 
	// Get and display the name of the computer. 
	GetComputerName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);
	strcat(header," ");
	GetUserName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);

	FILE *fptr = NULL;
	fptr = fopen(strFileName, "wb");
	if(fptr == NULL) return -1;

	// write the header
	fwrite(header, 1, 80, fptr);
	// write tri number
	int num_tri = TriNum;
	fwrite((char *)&num_tri, sizeof(int), 1, fptr);

	// write each triangle
	for(int i=0;i<num_tri;i++)
	{
		CTriangle& curTri = TriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}

	fclose(fptr);
	return num_tri;
}

int CTriangle::Combine_BinarySTL(int TriNum, CTriangle *TriArray, CString &strFileName)
{
	CTriangle *oldTriArray=NULL;
	int oldTriNum = Read_BinarySTLFile(strFileName, oldTriArray);

	char header[80] = "3DSystems Generated by YC ";
	LPTSTR lpszSystemInfo;     // pointer to system information string 
	DWORD cchBuff = 256;       // size of computer or user name 
	TCHAR tchBuffer2[512];
 	lpszSystemInfo = tchBuffer2; 
 
	// Get and display the name of the computer. 
	GetComputerName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);
	strcat(header," ");
	GetUserName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);

	FILE *fptr = NULL;
	fptr = fopen(strFileName, "wb");
	if(fptr == NULL) return -1;

	// write the header
	fwrite(header, 1, 80, fptr);
	// write tri number
	int num_tri = TriNum+oldTriNum;
	fwrite((char *)&num_tri, sizeof(int), 1, fptr);

	// write each old triangle
	for(int i=0;i<oldTriNum;i++)
	{
		CTriangle& curTri = oldTriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}
	if(oldTriArray!=NULL) delete []oldTriArray;

	// write each new triangle
	for(int i=0;i<TriNum;i++)
	{
		CTriangle& curTri = TriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}

	fclose(fptr);
	return num_tri;
}

int CTriangle::Combine_BinarySTL(vector<CTriangle> &TriArray, CString &strFileName)
{
	CTriangle *oldTriArray=NULL;
	int oldTriNum = Read_BinarySTLFile(strFileName, oldTriArray);
	int TriNum = (int)TriArray.size();

	char header[80] = "3DSystems Generated by YC ";
	LPTSTR lpszSystemInfo;     // pointer to system information string 
	DWORD cchBuff = 256;       // size of computer or user name 
	TCHAR tchBuffer2[512];
 	lpszSystemInfo = tchBuffer2; 
 
	// Get and display the name of the computer. 
	GetComputerName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);
	strcat(header," ");
	GetUserName(lpszSystemInfo, &cchBuff); 
	strcat(header,lpszSystemInfo);

	FILE *fptr = NULL;
	fptr = fopen(strFileName, "wb");
	if(fptr == NULL) return -1;

	// write the header
	fwrite(header, 1, 80, fptr);
	// write tri number
	int num_tri = TriNum+oldTriNum;
	fwrite((char *)&num_tri, sizeof(int), 1, fptr);

	// write each old triangle
	for(int i=0;i<oldTriNum;i++)
	{
		CTriangle& curTri = oldTriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}
	if(oldTriArray!=NULL) delete []oldTriArray;

	// write each new triangle
	for(int i=0;i<TriNum;i++)
	{
		CTriangle& curTri = TriArray[i];
		CVertex v1, v2, v3;
		v1 = curTri[0];	v2 = curTri[1];	v3 = curTri[2];
		// get normalized normal
		CVertex n = curTri.ComputeNormal();
		float temf[3];
		temf[0] = (float)n[0];	temf[1] = (float)n[1];	temf[2] = (float)n[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}

	fclose(fptr);
	return num_tri;
}

// return number of triangles
int CTriangle::Read_BinarySTLFile(CString &STLFileName, CTriangle *&pArray_Tri)
{
	FILE *fptr = NULL;
	fptr = fopen(STLFileName, "rb");
	if(fptr == NULL) return -1;

	// read the header
	char header[80];
	fread(header, 1, 80, fptr);
	// read Triangle number
	int num_tri = 0;
	fread((char *)&num_tri, sizeof(int), 1, fptr);
	if(num_tri <= 0) return 0;

	// read each triangle
	if(pArray_Tri != NULL) delete[] pArray_Tri;
	pArray_Tri = new CTriangle[num_tri];
	for(int i=0;i<num_tri;i++)
	{
		CVertex normal, v1, v2, v3;
		float temf[3];
		// read normal
		fread((char *)temf, sizeof(float), 3, fptr);
		normal[0] = temf[0];	normal[1] = temf[1];	normal[2] = temf[2];
#ifdef _DEBUG
		assert(_isnan(normal[0])==0 && _isnan(normal[1])==0 && _isnan(normal[2])==0);
#endif
		// read vertices
		fread((char *)temf, sizeof(float), 3, fptr);
		v1[0] = temf[0];	v1[1] = temf[1];	v1[2] = temf[2];
#ifdef _DEBUG
		assert(_isnan(v1[0])==0 && _isnan(v1[1])==0 && _isnan(v1[2])==0);
#endif
		fread((char *)temf, sizeof(float), 3, fptr);
		v2[0] = temf[0];	v2[1] = temf[1];	v2[2] = temf[2];
#ifdef _DEBUG
		assert(_isnan(v2[0])==0 && _isnan(v2[1])==0 && _isnan(v2[2])==0);
#endif
		fread((char *)temf, sizeof(float), 3, fptr);
		v3[0] = temf[0];	v3[1] = temf[1];	v3[2] = temf[2];
#ifdef _DEBUG
		assert(_isnan(v3[0])==0 && _isnan(v3[1])==0 && _isnan(v3[2])==0);
#endif
		// read attribute		
		short attrib = 0;
		fread((char *)&attrib, sizeof(short), 1, fptr);

		CTriangle curT(v1, v2, v3);
		///curT.SetNormal(normal);
		curT.ComputeNormal();
		curT.CalcArea();
		curT.SetTag(attrib);
		pArray_Tri[i]=curT;
	}

	fclose(fptr);
	return num_tri;
}

//////////////////////////////////////////////////////////////////////
// CTraingleID
// Instead of CVertex, this class only record the vertex index
//////////////////////////////////////////////////////////////////////

CTriangleID::CTriangleID(void)
{	
	mTag = 0;
	mColorID = -1;
	m_ShellNum = -1;
}

CTriangleID::CTriangleID(const CTriangleID &rhs)
{
	mTag = rhs.mTag;
	mColorID = rhs.mColorID;
	m_vert[0] = rhs.m_vert[0];
	m_vert[1] = rhs.m_vert[1];
	m_vert[2] = rhs.m_vert[2];
	m_Normal = rhs.m_Normal;
	m_Area = rhs.m_Area;
	m_ShellNum = rhs.m_ShellNum;
}

CTriangleID::CTriangleID(const int p0,const int p1,  const int p2)
{
	m_vert[0] = p0;
	m_vert[1] = p1;
	m_vert[2] = p2;
	mTag = 0;
	mColorID = -1;
	m_ShellNum = -1;
}      

CTriangleID& CTriangleID::operator =(const CTriangleID &rhs)
{
	mTag = rhs.mTag;
	mColorID = rhs.mColorID;
	m_vert[0] = rhs.m_vert[0];
	m_vert[1] = rhs.m_vert[1];
	m_vert[2] = rhs.m_vert[2];
	m_Normal = rhs.m_Normal;
	m_Area = rhs.m_Area;
	m_ShellNum = rhs.m_ShellNum;
	return *this;
}

int CTriangleID::operator [](int i)
{  
	return m_vert[i];
}

void CTriangleID::SetNormal(CVertex &aN)
{
	m_Normal = aN;
}

