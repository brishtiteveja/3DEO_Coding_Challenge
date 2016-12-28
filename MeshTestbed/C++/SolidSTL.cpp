//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// SolidSTL.cpp : Defines the initialization routines for the DLL.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxtempl.h>
#include <assert.h>
#include <math.h>

#include "gl.h"
#include "glu.h"

#include ".\vertex.h"
#include ".\triangle.h"
#include ".\SolidSTL.h"
#include ".\solidstl_builder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CHalfEdge
//////////////////////////////////////////////////////////////////////
CHalfEdge::CHalfEdge(int o, int r)
{
	opp_Vert = o;		
	rev_HEdge = r;
	mTag = -1;
	length = -1;
}

CHalfEdge::CHalfEdge(void)
{
	opp_Vert = -1;		
	rev_HEdge = -1;
	mTag = -1;
	length = -1;
}

CHalfEdge::CHalfEdge(const CHalfEdge &ahe)
{
	opp_Vert = ahe.opp_Vert;		
	rev_HEdge = ahe.rev_HEdge;
	mTag = ahe.mTag;
	length = ahe.length;
}

//////////////////////////////////////////////////////////////////////
// CTriVertex
//////////////////////////////////////////////////////////////////////
CTriVertex::CTriVertex(CVertex &aPos, CVertex &aNorm, int aValence)
{
	mVertPos = aPos;
	mVertNorm = aNorm;
	mVertValence = aValence;
	mNonFlatValence = 0;
}

CTriVertex::CTriVertex(CVertex &aPos)
{
	mVertPos = aPos;
	mVertValence = 0;
	mNonFlatValence = 0;
}

CTriVertex::CTriVertex(void)
{
	mVertValence = 0;
	mNonFlatValence = 0;
}

CTriVertex::CTriVertex(const CTriVertex &aTV)
{
	mVertPos = aTV.mVertPos;
	mVertNorm = aTV.mVertNorm;
	mVertValence = aTV.mVertValence;
	mNonFlatValence = aTV.mNonFlatValence;
}

//////////////////////////////////////////////////////////////////////
// SolidSTL
//////////////////////////////////////////////////////////////////////
int SolidSTL::lastDpyList = 111;	// to make sure no redundant in the display list

SolidSTL::SolidSTL(CString &aSTLFileName)
{
	m_STLFileName = aSTLFileName;
	mVertArray = NULL;
	mTriArray = NULL;
	mHedgeArray = NULL;

	mArrayBackupVNum = 100;
	mArrayBackupTNum = mArrayBackupVNum*2;
	mArrayBackupHENum = mArrayBackupTNum*3;

	m_modelID = 0;
	m_startDpyList = 0;
	m_endDpyList = 0;
}

SolidSTL::~SolidSTL(void)
{
	// delete the display list
	DeleteDpyLists();

	if(mVertArray != NULL) delete []mVertArray;
	if(mTriArray != NULL) delete []mTriArray;
	if(mHedgeArray != NULL) delete []mHedgeArray;
}

// call SolidCTL_Builder to complete all information from STL file
bool SolidSTL::Build(bool bSolidSTL, CProgressCtrl *aProgressCtrl)
{
	Clean();
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(5);
	SolidSTL_Builder sb;
	if(sb.Construct(m_STLFileName, this, bSolidSTL, aProgressCtrl)==false)
	{
		AfxMessageBox("This STL is bad! Please verify and fix it first before using the system."); 
		return false;	
	}
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(90);
	CreateDisplayList(); // create a display list for the part
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(100);
	return true;
}

// build by a list of triangles
bool SolidSTL::Build(int TriNum, CTriangle *pArray_Tri, CProgressCtrl *aProgressCtrl)
{
	Clean();
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(5);
	SolidSTL_Builder sb;
	if( sb.Construct(TriNum, pArray_Tri, this, aProgressCtrl)==true)
	{
		if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(90);
		CreateDisplayList(); // create a display list for the part
		if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(100);
		return true;
	}
	else 
		return false;
}

void SolidSTL::Clean(void)
{
	// delete the display list
	DeleteDpyLists();

	if(mVertArray != NULL) delete []mVertArray;
	if(mTriArray != NULL) delete []mTriArray;
	if(mHedgeArray != NULL) delete []mHedgeArray;
	mVertArray = NULL;
	mTriArray = NULL;
	mHedgeArray = NULL;

	mArrayBackupVNum = 100;
	mArrayBackupTNum = mArrayBackupVNum*2;
	mArrayBackupHENum = mArrayBackupTNum*3;

	m_modelID = 0;
	m_startDpyList = 0;
	m_endDpyList = 0;
}

CTriVertex& SolidSTL::Vert(int avNum)
{
#ifdef _DEBUG
	assert(avNum >= 0 && avNum < mVert_Num);
#endif
	return mVertArray[avNum];
}

CTriangleID& SolidSTL::Tri(int atNum)
{
#ifdef _DEBUG
	assert(atNum >= 0 && atNum < mTri_Num);
#endif
	return mTriArray[atNum];
}

CHalfEdge& SolidSTL::Hedge(int aeNum)
{
#ifdef _DEBUG
	assert(aeNum >= 0 && aeNum < mHE_Num);
#endif
	return mHedgeArray[aeNum];
}

void SolidSTL::GetBoundingBox(double *PartMin, double *PartMax)
{
	for(int i=0;i<3;i++)
	{
		PartMin[i] = m_PartMin[i];
		PartMax[i] = m_PartMax[i];
	}
}

CTriangle SolidSTL::getTri( int TriID )
{
	int v[3];
	v[0] = mTriArray[TriID][0];
	v[1] = mTriArray[TriID][1];
	v[2] = mTriArray[TriID][2];

	CTriangle resultT(mVertArray[v[0]].Position(), mVertArray[v[1]].Position(), mVertArray[v[2]].Position());
	return resultT;
}

#define EDGE_ATTRIB_TOL 1e-3

edgeAttrib SolidSTL::Judge_EdgeAttrib(int hedgeID)
{
	if(hedgeID < 0 || hedgeID >= mHE_Num) return (UNKNOWN_E);
	int rHE = mHedgeArray[hedgeID].R();
	if(rHE < 0 || rHE >= mHE_Num) return (UNKNOWN_E);
	int iO1, iO2;
	iO1 = mHedgeArray[hedgeID].O();
	iO2 = mHedgeArray[rHE].O();
	int iV1, iV2; 
	iV1 = startV(hedgeID);
	iV2 = endV(hedgeID);

	if( iO1 < 0 || iO1 >= mVert_Num ||
		iO2 < 0 || iO2 >= mVert_Num ||
		iV1 < 0 || iV1 >= mVert_Num ||
		iV2 < 0 || iV2 >= mVert_Num ) 
		return (UNKNOWN_E);

	CVertex pA, pB, pC, pD;	// two trianges: A->B->C and D->C->B
	pA = mVertArray[iV1].Position();
	pB = mVertArray[iV2].Position();
	pC = mVertArray[iO1].Position();
	pD = mVertArray[iO2].Position();

	CVertex AD, AB, AC, product;
	AD=pD-pA;
	AB=pB-pA;
	AC=pC-pA;
	product =AB.cross(AC);
	product.normalize();

	double result;
	result = AD.dot(product);

	if(result > EDGE_ATTRIB_TOL) return CONCAVE_E;
	if(result < -EDGE_ATTRIB_TOL) return CONVEX_E;
	return FLAT_E;
}

int SolidSTL::enext(int he)
{
#ifdef _DEBUG
	assert(he >= 0 && he < mHE_Num);
#endif
	return (he/3*3 + (he+1)%3);
}

int SolidSTL::eprev(int he)
{
#ifdef _DEBUG
	assert(he >= 0 && he < mHE_Num);
#endif
	return (he/3*3 + (he+2)%3);
}

int SolidSTL::startV(int he)	// e.n.o
{
	int en = enext(he);
	return mHedgeArray[en].O();
}

int SolidSTL::endV(int he)	// e.p.o
{
	int ep = eprev(he);
	return mHedgeArray[ep].O();
}

#define DISPLAY_LIST_SIZE 8192
void SolidSTL::CreateDisplayList(void)
{
	m_startDpyList = lastDpyList+1;
	GLuint dpyListNumber = m_startDpyList;

	for(int i=0; i<mTri_Num; i++)
	{
		CVertex v1 = mVertArray[mTriArray[i][0]].Position();
		CVertex v2 = mVertArray[mTriArray[i][1]].Position();
		CVertex v3 = mVertArray[mTriArray[i][2]].Position();
		CVertex Norm = mTriArray[i].Normal();

		if ((i % DISPLAY_LIST_SIZE) == 0)
		{
#ifdef _DEBUG
			assert(glIsList(dpyListNumber) == GL_FALSE);// a unique ID
#endif
			glNewList(dpyListNumber, GL_COMPILE);
			glBegin(GL_TRIANGLES);
		}
		// display method for FLAT shading 
		glNormal3f((float)Norm[AXIS_X], (float)Norm[AXIS_Y], (float)Norm[AXIS_Z]);
		glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		glVertex3f((float)v2[AXIS_X], (float)v2[AXIS_Y], (float)v2[AXIS_Z]);
		glVertex3f((float)v3[AXIS_X], (float)v3[AXIS_Y], (float)v3[AXIS_Z]);

		// display method for SMOOTH shading 
		//glNormal3f((float)v1_Norm[AXIS_X], (float)v1_Norm[AXIS_Y], (float)v1_Norm[AXIS_Z]);
		//glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		//glNormal3f((float)v2_Norm[AXIS_X], (float)v2_Norm[AXIS_Y], (float)v2_Norm[AXIS_Z]);
		//glVertex3f((float)v2[AXIS_X], (float)v2[AXIS_Y], (float)v2[AXIS_Z]);
		//glNormal3f((float)v3_Norm[AXIS_X], (float)v3_Norm[AXIS_Y], (float)v3_Norm[AXIS_Z]);
		//glVertex3f((float)v3[AXIS_X], (float)v3[AXIS_Y], (float)v3[AXIS_Z]);

		if ((i % DISPLAY_LIST_SIZE) == DISPLAY_LIST_SIZE-1 || i == mTri_Num-1)
		{
			glEnd();
			glEndList();
			dpyListNumber++;
		}
	}		

	m_endDpyList = dpyListNumber-1;
	lastDpyList = m_endDpyList;
}

void SolidSTL::DeleteDpyLists(void)
{
	if (m_startDpyList > 0)
	{
		glDeleteLists(m_startDpyList,  m_endDpyList - m_startDpyList + 1);
		m_startDpyList = m_endDpyList = 0;
	}
}

void SolidSTL::UpdateDisplayList(void)
{
	DeleteDpyLists();
	CreateDisplayList();
}

void SolidSTL::CalcExtents(void)
{
	double MinX=1e10, MinY=1e10, MinZ=1e10;
	double MaxX=-1e10, MaxY=-1e10, MaxZ=-1e10;
	for (int i=0; i<mVert_Num; i++)
    {
		// get the bounding box
		CVertex curV = Vert(i).Position();
		CTransMatrix modelMatrix;
		if(m_MatrixList.IsEmpty() == FALSE) 
			modelMatrix= m_MatrixList.GetTail();
		float inV[3], outV[3];
		inV[0] = curV[0];	inV[1] = curV[1];	inV[2] = curV[2];
		modelMatrix.GetTransformedVertex(inV,outV);

		if(outV[AXIS_X] < MinX) MinX = outV[AXIS_X];
		if(outV[AXIS_Y] < MinY) MinY = outV[AXIS_Y];
		if(outV[AXIS_Z] < MinZ) MinZ = outV[AXIS_Z];
		if(outV[AXIS_X] > MaxX) MaxX = outV[AXIS_X];
		if(outV[AXIS_Y] > MaxY) MaxY = outV[AXIS_Y];
		if(outV[AXIS_Z] > MaxZ) MaxZ = outV[AXIS_Z];
	}
	m_PartMin[AXIS_X] = MinX;
	m_PartMin[AXIS_Y] = MinY;
	m_PartMin[AXIS_Z] = MinZ;
	m_PartMax[AXIS_X] = MaxX;
	m_PartMax[AXIS_Y] = MaxY;
	m_PartMax[AXIS_Z] = MaxZ;
}

double SolidSTL::CalcArea(void)
{
	double total=0.0;
	double curTri;
	for (int i=0; i<mTri_Num; i++)
	{
		curTri = mTriArray[i].Area();
		total += curTri;
	}
	return total;
}

double SolidSTL::CalcVolume(void)
{
	double total=0.0;
	for (int i=0; i<mTri_Num; i++)
	{
		CVertex v1 = mVertArray[mTriArray[i][0]].Position();
		CVertex v2 = mVertArray[mTriArray[i][1]].Position();
		CVertex v3 = mVertArray[mTriArray[i][2]].Position();
		total +=
			v1[AXIS_X]*v2[AXIS_Y]*v3[AXIS_Z] + v1[AXIS_Y]*v2[AXIS_Z]*v3[AXIS_X] + v1[AXIS_Z]*v2[AXIS_X]*v3[AXIS_Y] - 
			v1[AXIS_Z]*v2[AXIS_Y]*v3[AXIS_X] - v1[AXIS_Y]*v2[AXIS_X]*v3[AXIS_Z] - v1[AXIS_X]*v2[AXIS_Z]*v3[AXIS_Y];
	}
	return total/6.0;
}

void SolidSTL::rotateX(double theta)
{
	theta = theta * (M_PI/180.0);
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	for (int i=0;i<mVert_Num;i++)
	{
		// change position
		CVertex &curVert = Vert(i).Position();
		double y = curVert[AXIS_Y];
		double z = curVert[AXIS_Z];
		curVert[AXIS_Y] = (y*cosTheta - z*sinTheta);
		curVert[AXIS_Z] = (y*sinTheta + z*cosTheta);
		// change normal of vertex
		CVertex &curNorm = Vert(i).Normal();
		y = curNorm[AXIS_Y];
		z = curNorm[AXIS_Z];
		curNorm[AXIS_Y] = (y*cosTheta - z*sinTheta);
		curNorm[AXIS_Z] = (y*sinTheta + z*cosTheta);
	}
	// change normal of triangles
	for (int i=0;i<mTri_Num;i++)
	{
		// change position
		CVertex &curNorm = Tri(i).Normal();
		double y = curNorm[AXIS_Y];
		double z = curNorm[AXIS_Z];
		curNorm[AXIS_Y] = (y*cosTheta - z*sinTheta);
		curNorm[AXIS_Z] = (y*sinTheta + z*cosTheta);
	}

	CalcExtents();
}

void SolidSTL::rotateY(double theta)
{
	theta = theta * (M_PI/180.0);
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	for (int i=0;i<mVert_Num;i++)
	{
		// change position
		CVertex &curVert = Vert(i).Position();
		double x = curVert[AXIS_X];
		double z = curVert[AXIS_Z];
		curVert[AXIS_X] = (x*cosTheta + z*sinTheta);
		curVert[AXIS_Z] = (x*sinTheta - z*cosTheta);
		// change normal of vertex
		CVertex &curNorm = Vert(i).Normal();
		x = curNorm[AXIS_X];
		z = curNorm[AXIS_Z];
		curNorm[AXIS_X] = (x*cosTheta + z*sinTheta);
		curNorm[AXIS_Z] = (x*sinTheta - z*cosTheta);
	}
	// change normal of triangles
	for (int i=0;i<mTri_Num;i++)
	{
		// change position
		CVertex &curNorm = Tri(i).Normal();
		double x = curNorm[AXIS_X];
		double z = curNorm[AXIS_Z];
		curNorm[AXIS_X] = (x*cosTheta + z*sinTheta);
		curNorm[AXIS_Z] = (x*sinTheta - z*cosTheta);
	}

	CalcExtents();
}

void SolidSTL::rotateZ(double theta)
{
	theta = theta * (M_PI/180.0);
	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	for (int i=0;i<mVert_Num;i++)
	{
		// change position
		CVertex &curVert = Vert(i).Position();
		double x = curVert[AXIS_X];
		double y = curVert[AXIS_Y];
		curVert[AXIS_X] = (x*cosTheta - y*sinTheta);
		curVert[AXIS_Y] = (x*sinTheta + y*cosTheta);
		// change normal of vertex
		CVertex &curNorm = Vert(i).Normal();
		x = curNorm[AXIS_X];
		y = curNorm[AXIS_Y];
		curNorm[AXIS_X] = (x*cosTheta - y*sinTheta);
		curNorm[AXIS_Y] = (x*sinTheta + y*cosTheta);
	}
	// change normal of triangles
	for (int i=0;i<mTri_Num;i++)
	{
		// change position
		CVertex &curNorm = Tri(i).Normal();
		double x = curNorm[AXIS_X];
		double y = curNorm[AXIS_Y];
		curNorm[AXIS_X] = (x*cosTheta - y*sinTheta);
		curNorm[AXIS_Y] = (x*sinTheta + y*cosTheta);
	}

	CalcExtents();
}

bool SolidSTL::saveSTL(CString &aFileName)
{
	FILE *fptr = NULL;
	fptr = fopen(aFileName, "wb");
	if(fptr == NULL) return false;
	//if (ftell(fp) == 0)	Read_stl_Binary(fp);
	//else Read_stl_Text(fp);

	// write the header
	fwrite(m_Header, 1, 80, fptr);
	// write Triangle number
	fwrite((char *)&mTri_Num, sizeof(int), 1, fptr);

	// write each triangle
	for(int i=0;i<mTri_Num;i++)
	{
		CTriangle curT=getTri(i);
		CVertex &normal = curT.Normal();
		CVertex &v1 = curT[0];		
		CVertex &v2 = curT[1];		
		CVertex &v3 = curT[2];

		float temf[3];
		// write normal
		temf[0] = (float)normal[0];	temf[1] = (float)normal[1];	temf[2] = (float)normal[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);

		// write vertices
		temf[0] = (float)v1[0];	temf[1] = (float)v1[1];	temf[2] = (float)v1[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);

		temf[0] = (float)v2[0];	temf[1] = (float)v2[1];	temf[2] = (float)v2[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);

		temf[0] = (float)v3[0];	temf[1] = (float)v3[1];	temf[2] = (float)v3[2];
		fwrite((char *)temf, sizeof(float), 3, fptr);

		// write attribute		
		short attrib = 0;
		fwrite((char *)&attrib, sizeof(short), 1, fptr);
	}

	fclose(fptr);
	return true;
}

