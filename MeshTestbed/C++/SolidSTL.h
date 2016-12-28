//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
// SolidSTL.h : main header file for the SolidSTL DLL
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <afxtempl.h>
#include ".\TransMatrix.h"

enum edgeAttrib { UNKNOWN_E, CONVEX_E, FLAT_E, CONCAVE_E  };
enum triAttrib { UNKNOWN_T, CONVEX_T, CONCAVE_T };

class CHalfEdge
{
	friend class SolidSTL;
	friend class SolidSTL_Builder;
protected:
	int opp_Vert;		// opposite vertex number
	int rev_HEdge;		// reverse half edge number
	int mTag;			
	double length;		// length of the edge
	bool bFlat;			

public:
	CHalfEdge(int o, int r);
	CHalfEdge(void);
	CHalfEdge(const CHalfEdge &ahe);
	~CHalfEdge(void) {}

	void SetO(int aO) {	opp_Vert = aO;	}
	int O() {	return opp_Vert;	}
	void SetR(int aR) {	rev_HEdge = aR;	}
	int R() {	return rev_HEdge;	}
	void SetTag(int atag) {	mTag = atag;	}
	int tag() {	return mTag;	}
	void SetLen(double alen) {	length = alen;	}
	double len() {	return length;	}
	bool Flat() { return bFlat; }
};

class CTriVertex
{
	friend class SolidSTL;
protected:
	CVertex mVertPos;
	CVertex mVertNorm;
	int mVertValence;	// the valence of each vertex
	int mNonFlatValence;	// the valence of each vertex whose edges are not flat
public:
	int IncidentEdgeNum;

public:
	CTriVertex(CVertex &aPos, CVertex &aNorm, int aValence);
	CTriVertex(CVertex &aPos);
	CTriVertex(void);
	CTriVertex(const CTriVertex &aTV);
	~CTriVertex(void) {}

	void SetPosition(CVertex &aP) {	mVertPos = aP;	}
	CVertex &Position() {	return mVertPos;	}

	void SetNormal(CVertex &aN) {	mVertNorm = aN;	}
	CVertex &Normal() {	return mVertNorm;	}

	void SetValence(int aV) {	mVertValence = aV;	}
	int &Valence() {	return mVertValence;	}
	int &NonFlatValence() {	return mNonFlatValence;	}
};

class SolidSTL  
{
	friend class SolidSTL_Builder;
	friend class SolidSTL_Viewer;
	friend class SolidSTL_Decimator;

protected:
	CString m_STLFileName;
	char m_Header[80];
	CList <CTransMatrix, CTransMatrix &> m_MatrixList;	// list of transformation
	int m_modelID;	// used for selecting STL
	
	int mHandle_Num;
	// vertice info
	int mVert_Num;
	int mArrayBackupVNum; // we need some back up elements in the vertices array 

	CTriVertex *mVertArray;

	// triangle info
	int mTri_Num;
	int mArrayBackupTNum; // we need some back up elements in the triangles array 
	CTriangleID *mTriArray;

	// half edges info.
	int mHE_Num;
	int mArrayBackupHENum;// we need some back up elements in the Halfedge array 
	CHalfEdge *mHedgeArray;
	
	// extends
	double m_PartMin[3], m_PartMax[3];

	static int lastDpyList;	// a counter
	int m_startDpyList;
	int m_endDpyList;
	void CreateDisplayList(void);
	void DeleteDpyLists(void);
public:
	void UpdateDisplayList(void);
	CList <CTransMatrix, CTransMatrix &> &getTransformList(void)
	{
		return m_MatrixList;
	}
public:
	SolidSTL(CString &aSTLFileName);
	~SolidSTL(void);
	// call SolidCTL_Builder to complete all information
	bool Build(bool bSolidSTL, CProgressCtrl *aProgressCtrl=NULL);
	bool Build(int TriNum, CTriangle *pArray_Tri, CProgressCtrl *aProgressCtrl=NULL);
	bool saveSTL(CString &aFileName);
	CString &getFileName(void)	{ return m_STLFileName;	}

	int TriNum()		{	return mTri_Num;	}
	int VertexNum()		{	return mVert_Num;	}
	int HalfEdgeNum()	{	return mHE_Num;		}
	int HandleNum()		{	return mHandle_Num;	}
	CTriVertex& Vert(int avNum);
	CTriangleID& Tri(int atNum);
	CHalfEdge& Hedge(int aeNum);

	int GetModelID()	{	return m_modelID;	};
	void SetModelID(int modelID)	{ m_modelID = modelID;	}

	// Manipulation functions
	CTriangle getTri( int TriID);
	edgeAttrib Judge_EdgeAttrib(int hedgeID);

	// get the next edge in the halfedge table, that is, e.n
	int enext(int he);
	// get the previous edge in the halfedge table, that is e.p or e.n.n
	int eprev(int he);
	// get the start vertex of the halfedge, that is, e.sv
	int startV(int he);
	// get the end vertex of the halfedge, that is e.ev
	int endV(int he);

	// caculation functions
	void GetBoundingBox(double *PartMin, double *PartMax);
	void CalcExtents(void);

	double CalcArea(void);
	double CalcVolume(void);

	// geometry transformation
	void rotateX(double theta);
	void rotateY(double theta);
	void rotateZ(double theta);

protected:
	void Clean(void);
};
