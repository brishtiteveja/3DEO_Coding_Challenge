//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <afxtempl.h>
#include <float.h>
#include <assert.h>

#include ".\vertex.h"
#include ".\triangle.h"
#include ".\solidstl.h"
#include ".\solidstl_builder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CSortVertex
//////////////////////////////////////////////////////////////////////
CSortVertex::CSortVertex(CVertex &avec, int aTriNum, int aVertNum)
{
	m_vec = avec;
	mTriNum = aTriNum;
	mVertNum = aVertNum;
}

CSortVertex::CSortVertex()
{
	mTriNum = -1;
	mVertNum = -1;
}

CSortVertex::CSortVertex(const CSortVertex &asv)
{
	m_vec = asv.m_vec;
	mTriNum = asv.mTriNum;
	mVertNum = asv.mVertNum;
}

void CSortVertex::operator =(const CSortVertex &asv)
{
	m_vec = asv.m_vec;
	mTriNum = asv.mTriNum;
	mVertNum = asv.mVertNum;
}

void CSortVertex::quick_sort_vertex_array(CSortVertex *tempArray_vertices, int low, int high)
{
	CArray <int, int> SortRanges;
	SortRanges.Add(low);
	SortRanges.Add(high);

	while(SortRanges.GetSize() > 0)		// not all are processed
	{
		int curLow = SortRanges[0];
		int curHigh = SortRanges[1];
		SortRanges.RemoveAt(0, 2);

		int pivot,i,j;	
		pivot = curLow;
		i=curLow+1;
		j=curHigh;

		if(curLow >= curHigh) continue;
		if(curHigh - curLow == 1)
		{
			if(CSortVertex::compare_vertex_with_tolerance(tempArray_vertices, curLow, curHigh) > 0)
				CSortVertex::exchange_vertex(tempArray_vertices, curLow, curHigh);
			continue;
		}

		while(1)
		{
			while(CSortVertex::compare_vertex_with_tolerance(tempArray_vertices, pivot, j) <= 0 
					&& j>pivot) 
				j--;
			if(j<i) break;
			CSortVertex::exchange_vertex(tempArray_vertices, pivot, j);
			pivot = j;
			//printf("swap: pivot: %d j: %d\n", pivot, j);
			while(CSortVertex::compare_vertex_with_tolerance(tempArray_vertices, j, i) >= 0 
					&& i<j) 
				i++; 
			if(i>=j) break;
			CSortVertex::exchange_vertex(tempArray_vertices, j, i);
			//printf("swap: pivot: %d i: %d\n", pivot, i);
			pivot = i;
			i++;
			j--;
		}

		SortRanges.Add(curLow);
		SortRanges.Add(pivot-1);

		SortRanges.Add(pivot+1);
		SortRanges.Add(curHigh);
	}
}

void CSortVertex::exchange_vertex(CSortVertex *tempArray_vertices, int v1, int v2)
{
	CSortVertex tmp1, tmp2;	
	tmp1 = tempArray_vertices[v1];
	tmp2 = tempArray_vertices[v2];

	tempArray_vertices[v1] = tmp2;
	tempArray_vertices[v2] = tmp1;
}

// e1 > e2, return 1;
// e1 == e2, return 0;
// e1 < e2, return -1
int CSortVertex::compare_vertex(CSortVertex *tempArray_vertices, int v1, int v2)
{
	CSortVertex tmp1 = tempArray_vertices[v1];
	CSortVertex tmp2 = tempArray_vertices[v2];

	if(tmp1.m_vec < tmp2.m_vec)	return -1;	
	if(tmp1.m_vec > tmp2.m_vec)	return 1;

	return 0;
}

// e1 > e2, return 1;
// e1 == e2, return 0;
// e1 < e2, return -1
int CSortVertex::compare_vertex_with_tolerance(CSortVertex *tempArray_vertices, int v1, int v2)
{
	CSortVertex tmp1 = tempArray_vertices[v1];
	CSortVertex tmp2 = tempArray_vertices[v2];
	if(tmp1.m_vec == tmp2.m_vec) return 0;
	else if(tmp1.m_vec >= tmp2.m_vec) return 1;
	else return -1;	
}

//////////////////////////////////////////////////////////////////////
// CSortEdge
//////////////////////////////////////////////////////////////////////
CSortEdge::CSortEdge(int v1, int v2, int ENum)
{
	mv1 = v1; 
	mv2 = v2;
	mENum = ENum;
}

CSortEdge::CSortEdge()
{
	mv1 = -1; 
	mv2 = -1;
	mENum = -1;
}

CSortEdge::CSortEdge(const CSortEdge &asv)
{
	mv1 = asv.mv1; 
	mv2 = asv.mv2;
	mENum = asv.mENum;
}

void CSortEdge::operator =(const CSortEdge &asv)
{
	mv1 = asv.mv1; 
	mv2 = asv.mv2;
	mENum = asv.mENum;
}

void CSortEdge::quick_sort_edge_array(CSortEdge *tempArray_edges, int low, int high)
{
	CArray <int, int> SortRanges;
	SortRanges.Add(low);
	SortRanges.Add(high);

	while(SortRanges.GetSize() > 0)		// not all are processed
	{
		int curLow = SortRanges[0];
		int curHigh = SortRanges[1];
		SortRanges.RemoveAt(0, 2);

		int pivot,i,j;	
		pivot = curLow;
		i=curLow+1;
		j=curHigh;

		if(curLow >= curHigh) continue;
		if(curHigh - curLow == 1)
		{
			if(CSortEdge::compare_edge(tempArray_edges, curLow, curHigh) > 0)
				CSortEdge::exchange_edge(tempArray_edges, curLow, curHigh);
			continue;
		}

		while(1)
		{
			while(CSortEdge::compare_edge(tempArray_edges, pivot, j) < 0) 
				j--;
			if(j<i) break;
			CSortEdge::exchange_edge(tempArray_edges, pivot, j);
			pivot = j;
			//printf("swap: pivot: %d j: %d\n", pivot, j);
			while(CSortEdge::compare_edge(tempArray_edges, j, i) > 0) 
				i++; 
			if(i>=j) break;
			CSortEdge::exchange_edge(tempArray_edges, j, i);
			//printf("swap: pivot: %d i: %d\n", pivot, i);
			pivot = i;
			i++;
			j--;
		}

		SortRanges.Add(curLow);
		SortRanges.Add(pivot-1);

		SortRanges.Add(pivot+1);
		SortRanges.Add(curHigh);
	}
}

void CSortEdge::exchange_edge(CSortEdge *tempArray_edges, int v1, int v2)
{
	CSortEdge tmp1, tmp2;	
	tmp1 = tempArray_edges[v1];
	tmp2 = tempArray_edges[v2];

	tempArray_edges[v1] = tmp2;
	tempArray_edges[v2] = tmp1;
}

// e1 > e2, return 1;
// e1 == e2, return 0;
// e1 < e2, return -1
int CSortEdge::compare_edge(CSortEdge *tempArray_edges, int v1, int v2)
{
	CSortEdge tmp1 = tempArray_edges[v1];
	CSortEdge tmp2 = tempArray_edges[v2];

	if(tmp1.mv1 < tmp2.mv1)	return -1;	
	if(tmp1.mv1 > tmp2.mv1)	return 1;	
	
	if(tmp1.mv2 < tmp2.mv2)	return -1;
	if(tmp1.mv2 > tmp2.mv2)	return 1;

	return 0;
}



//////////////////////////////////////////////////////////////////////
// SolidSTL_Builder
//////////////////////////////////////////////////////////////////////
SolidSTL_Builder::SolidSTL_Builder(void)
{
	mp_solid = NULL;
}

SolidSTL_Builder::~SolidSTL_Builder(void)
{
}

// return the data related to the STL file
bool SolidSTL_Builder::Construct(CString &STLFileName, SolidSTL *ap_solid, 
								 bool bSolidSTL, CProgressCtrl *aProgressCtrl)
{
	if(ap_solid == NULL) return false;
	mp_solid = ap_solid;

	// Step 1: Load in STL
	CTriangle *pArray_Tri=NULL;		// temporary triangle table to store STL
	if(Read_STL_File(STLFileName, pArray_Tri) <= 0) return false;
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(30);
	
	// check and delete non-regular triangles
	if( bSolidSTL ) 
		mp_solid->mTri_Num = RegulateTriangles(mp_solid->mTri_Num, pArray_Tri);

	// Step 2: Convert to CTL
	// a temp vertex array since we do not know the number of vertices yet
	CArray <CVertex, CVertex&> VertArray;
	VertArray.SetSize(0, 1024);
	mp_solid->mTriArray = new CTriangleID[mp_solid->mTri_Num + mp_solid->mArrayBackupTNum];
	if(Build_CTL(pArray_Tri, VertArray, mp_solid->mTriArray) != true) return false;
	if(pArray_Tri != NULL) delete[] pArray_Tri;
	pArray_Tri = NULL;
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(60);

	mp_solid->mVertArray = new CTriVertex[mp_solid->mVert_Num + mp_solid->mArrayBackupVNum];
	for(int i=0;i<mp_solid->mVert_Num;i++)
		mp_solid->mVertArray[i].SetPosition(VertArray[i]);
	mp_solid->CalcExtents();	// get the bounding box
	VertArray.RemoveAll();	// no use any more

	// Step 3: Build half-edge table
	if(Build_Topology() == true) // topology of STL is good 
	{
		// Step 4: Check the STL valid
		int shellNum = CheckShellInfo();	// number of shells
		if(shellNum > 1 && bSolidSTL) 
		{
			AfxMessageBox("This STL has more than one shell! It is not supported by the system.");
		//	return false;	
		}

		// Step 5: get the length and curvature of half edges
		CalcHedgeInfo();	 

		// Step 6: get the normal and valence of vertices
		CalcVertexInfo();
	}
	else
	{
		if( bSolidSTL ) 
			AfxMessageBox("This STL is bad! Please verify and fix it first."); 
	}

	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(80);

	return true;
}

// return the data related to the STL file
bool SolidSTL_Builder::Construct(int TriNum, CTriangle *pArray_Tri,
								 SolidSTL *ap_solid, CProgressCtrl *aProgressCtrl)
{
	if(ap_solid == NULL) return false;
	mp_solid = ap_solid;

	// Step 1: Load in STL
	mp_solid->mTri_Num = TriNum;
	// check and delete non-regular triangles
	mp_solid->mTri_Num = RegulateTriangles(TriNum, pArray_Tri);

	// calculate normal of each triangle
	//for(int i=0;i<TriNum;i++) pArray_Tri[i].ComputeNormal();
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(30);

	// Step 2: Convert to CTL
	// a temp vertex array since we do not know the number of vertices yet
	CArray <CVertex, CVertex&> VertArray;
	VertArray.SetSize(0, 1024);
	mp_solid->mTriArray = new CTriangleID[mp_solid->mTri_Num + mp_solid->mArrayBackupTNum];
	if(Build_CTL(pArray_Tri, VertArray, mp_solid->mTriArray) != true) return false;
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(60);

	mp_solid->mVertArray = new CTriVertex[mp_solid->mVert_Num + mp_solid->mArrayBackupVNum];
	for(int i=0;i<mp_solid->mVert_Num;i++)
		mp_solid->mVertArray[i].SetPosition(VertArray[i]);
	mp_solid->CalcExtents();	// get the bounding box
	VertArray.RemoveAll();	// no use any more

	// Step 3: Build half-edge table
	if(Build_Topology() == true) // topology of STL is good 
	{
		// Step 4: Check the STL valid
		int shellNum = CheckShellInfo();	// number of shells
		//if(shellNum > 1) 
		//{
		//	AfxMessageBox("This STL has more than one shell! It is not supported by TSGS.");
		//	return false;	
		//}
        
		// Step 5: get the length and curvature of half edges
		CalcHedgeInfo();	 

		// Step 6: get the normal and valence of vertices
		CalcVertexInfo(); 
	}
	if(aProgressCtrl!=NULL) aProgressCtrl->SetPos(80);
	return true;
}

void SolidSTL_Builder::CalcVertexInfo()
{
	// initialize
	for(int i=0;i<mp_solid->mHE_Num;i++)
		mp_solid->mHedgeArray[i].SetTag(-1);
	for(int i=0;i<mp_solid->mVert_Num;i++)
	{
		CVertex av(0.0, 0.0, 0.0);
		mp_solid->mVertArray[i].SetNormal(av);
		mp_solid->mVertArray[i].SetValence(0);	
	}
	for(int i=0;i<mp_solid->mHE_Num;i++)
	{
		int StartV = mp_solid->startV(i);
		if(mp_solid->mVertArray[StartV].IncidentEdgeNum < 0)
			mp_solid->mVertArray[StartV].IncidentEdgeNum = i;			
	}
	for(int i=0;i<mp_solid->mHE_Num;i++)
	{
		if(mp_solid->mHedgeArray[i].tag() == -1)	// not processed
		{
			propagate_triangle_vertex(i);
		}
	}

	// normalization
	for(int i=0;i<mp_solid->mVert_Num;i++)
	{
		mp_solid->mVertArray[i].Normal().normalize();
	}
}

void SolidSTL_Builder::propagate_triangle_vertex(int root)
{
	CArray <int, int> ProcessHE;
	ProcessHE.Add(root);

	while(ProcessHE.GetSize() > 0)		// not all are processed
	{
		int curRoot = ProcessHE[0];
		ProcessHE.RemoveAt(0);
		if(mp_solid->mHedgeArray[curRoot].tag() != -1) continue;	// already processed
		
		int e1 = (curRoot/3)*3;
		mp_solid->mHedgeArray[e1].SetTag(1);	// set as processed
		mp_solid->mHedgeArray[e1+1].SetTag(1);	// set as processed
		mp_solid->mHedgeArray[e1+2].SetTag(1);	// set as processed

		// process the triangle
		int t1 = e1/3;
		CVertex TriNorm = mp_solid->mTriArray[t1].Normal();
		for(int kk = 0;kk<3; kk++)
		{
			int curv = mp_solid->mTriArray[t1][kk];
			CVertex &VertNorm = mp_solid->mVertArray[curv].Normal();
			VertNorm += TriNorm;
			mp_solid->mVertArray[curv].Valence()++;	
		}
 		for(int i=e1;i<e1+3;i++)
		{
			if(mp_solid->mHedgeArray[i].Flat() == false) 
			{
				int iStartV = mp_solid->startV(i);
				mp_solid->mVertArray[iStartV].NonFlatValence()++;
			}
		}

		for(int i=e1;i<e1+3;i++)
		{
			if(mp_solid->mHedgeArray[i].R()== -1) continue;
			ProcessHE.Add(mp_solid->mHedgeArray[i].R());
		}
	}
}

#define NORMAL_DIFFER 0.9999
void SolidSTL_Builder::CalcHedgeInfo()	
{
	// length of half edges
	for(int cure=0; cure < mp_solid->mHE_Num; cure++)
	{
		if(mp_solid->mHedgeArray[cure].len() >= 0) continue;	// already processed
		int sv, ev;
		sv = mp_solid->startV(cure);
		ev = mp_solid->endV(cure);
		CVertex sv_ev = mp_solid->mVertArray[ev].Position()-mp_solid->mVertArray[sv].Position();
		double eLen = sv_ev.length();

		mp_solid->mHedgeArray[cure].SetLen(eLen);
		int reve = mp_solid->mHedgeArray[cure].R();
		if(reve < 0 || reve >= mp_solid->HalfEdgeNum()) continue;
		mp_solid->mHedgeArray[reve].SetLen(eLen);

		int iT1, iT2;
		iT1 = cure/3;
		iT2 = reve/3;
		if(mp_solid->mTriArray[iT1].Normal().dot(mp_solid->mTriArray[iT2].Normal()) >= NORMAL_DIFFER)
		{	
			mp_solid->mHedgeArray[cure].bFlat = true;
			mp_solid->mHedgeArray[reve].bFlat = true;
		}
		else
		{
			mp_solid->mHedgeArray[cure].bFlat = false;
			mp_solid->mHedgeArray[reve].bFlat = false;
		}
	}
}

// return number of shells
int SolidSTL_Builder::CheckShellInfo()
{
	int shell_num = 0;
	for(int i=0;i<mp_solid->mHE_Num;i++)
	{
		if(mp_solid->mHedgeArray[i].tag() == -1)	// not processed
		{
			propagate_shell_triangle(i, shell_num);
			shell_num++;
		}
	}

	// calculate hanlde number of the part
	mp_solid->mHandle_Num = ((mp_solid->mTri_Num - 2*mp_solid->mVert_Num +4)/4);

	return shell_num;
}

void SolidSTL_Builder::propagate_shell_triangle(int root, int aShell)
{

	CArray <int, int> ProcessHE;
	ProcessHE.Add(root);

	while(ProcessHE.GetSize() > 0)		// not all are processed
	{
		int curRoot = ProcessHE[0];
		ProcessHE.RemoveAt(0);
		if(mp_solid->mHedgeArray[curRoot].tag() != -1) continue;	// already processed
		
		int e1 = (curRoot/3)*3;
		// process the triangle
		mp_solid->mHedgeArray[e1].SetTag(aShell);
		mp_solid->mHedgeArray[e1+1].SetTag(aShell);
		mp_solid->mHedgeArray[e1+2].SetTag(aShell);

		int t1 = e1/3;
		mp_solid->mTriArray[t1].SetShellNum(aShell);
	  
		for(int i=e1;i<e1+3;i++)
		{
			if(mp_solid->mHedgeArray[i].R()== -1) continue;
			ProcessHE.Add(mp_solid->mHedgeArray[i].R());
		}
	}
}

bool SolidSTL_Builder::Build_Topology()
{
	// get the half-edge table
	mp_solid->mHE_Num = 3*mp_solid->mTri_Num;
	mp_solid->mHedgeArray = new CHalfEdge[mp_solid->mHE_Num + mp_solid->mArrayBackupHENum];
	
	// a temp edge arrays to get reverse half-edges
	CSortEdge *tempArray_hedges = new CSortEdge[mp_solid->mHE_Num];
	// reading edge information
	for(int i=0;i<mp_solid->mTri_Num;i++)
	{
		int v[3];
		v[0] = mp_solid->mTriArray[i][0];
		v[1] = mp_solid->mTriArray[i][1];
		v[2] = mp_solid->mTriArray[i][2];
		for(int j=0;j<3;j++)
		{
			CSortEdge curSE;
			// flip v1, v2 if v1 > v2 
			if(v[j] < v[(j+1)%3])
			{
				curSE.mv1 = v[j];
				curSE.mv2 = v[(j+1)%3];
			}
			else
			{
				curSE.mv1 = v[(j+1)%3];
				curSE.mv2 = v[j];
			}			
			curSE.mENum = i*3+j;	// mENum is used to get reverse Half-Edge number
			tempArray_hedges[i*3+j] = curSE;
			
			CHalfEdge curHE(v[(j+2)%3], -1);
			mp_solid->mHedgeArray[i*3+j] = curHE;
		}
	}

	// quick order the edges
	CSortEdge::quick_sort_edge_array(tempArray_hedges, 0, mp_solid->mHE_Num-1);

	// fill rev_HEdge of half-edge table
	int unmatched = 0;	// record how many edges are unmatched
	for(int i=0; i < mp_solid->mHE_Num; i++)
	{
		if( i+1 < mp_solid->mHE_Num && 
			CSortEdge::compare_edge(tempArray_hedges, i, i+1) == 0)
		{
			// set reverse half edge
			int e1 = tempArray_hedges[i].mENum;
			int e2 = tempArray_hedges[i+1].mENum;	
			mp_solid->mHedgeArray[e1].SetR(e2);
			mp_solid->mHedgeArray[e2].SetR(e1);
			i++;
		}
		else 
		{
			int e1 = tempArray_hedges[i].mENum;
			mp_solid->mHedgeArray[e1].SetR(-1);
			unmatched++;
		}
	}

	if(tempArray_hedges != NULL) delete []tempArray_hedges;

	if(unmatched != 0) 
	{	
#ifdef _DEBUG
		//AfxMessageBox("This STL is bad! Please verify and fix it first."); 
#endif
		return false;	
	}

	return true;
}

// from a CTriangle array, generate a CVertex Array and a CTriangleID array
bool SolidSTL_Builder::Build_CTL(CTriangle *&pArray_Tri, 
								 CArray <CVertex, CVertex&> &VertArray, 
								 CTriangleID *pTriIDArray)
{
	// get the vertices table
	int vert_Num = 3*mp_solid->mTri_Num;
	CSortVertex *tempArray_vertices = new CSortVertex[vert_Num];
	for(int i=0; i < mp_solid->mTri_Num;i++)
	{
		CTriangle curT = pArray_Tri[i];
		CSortVertex sv1(curT[0], i, 0);
		tempArray_vertices[i*3] = sv1;
		CSortVertex sv2(curT[1], i, 1);
		tempArray_vertices[i*3+1] = sv2;
		CSortVertex sv3(curT[2], i, 2);
		tempArray_vertices[i*3+2] = sv3;
	}
	// quick order the vertices
	CSortVertex::quick_sort_vertex_array(tempArray_vertices, 0, vert_Num-1);

	// build vertices table and TriangleID table
	for(int i=0; i < mp_solid->mTri_Num;i++)
	{
		CTriangle curT = pArray_Tri[i];
		pTriIDArray[i].SetNormal(curT.Normal());
		/*
		char str[256];
		CVertex Norm = curT.Normal();
		sprintf_s(str, "Norm (x, y, z): (%lf, %lf, %lf)\n", Norm.getVec()[0], Norm.getVec()[1], Norm.getVec()[2]);
		OutputDebugString(str);
		CVertex Centroid = curT.Centroid();
		sprintf_s(str, "Centroid (x, y, z): (%lf, %lf, %lf)\n", Centroid.getVec()[0], Centroid.getVec()[1], Centroid.getVec()[2]);
		OutputDebugString(str);
		*/

		pTriIDArray[i].SetCentroid(curT.Centroid());
		pTriIDArray[i].SetArea(curT.Area());
		pTriIDArray[i].SetSlope(curT.Slope());
		pTriIDArray[i].SetTag(curT.tag());
	}
	int curVert_Num = -1;
	for(int i=0;i<vert_Num;i++)
	{
		// check to add new vertex
		if(curVert_Num < 0 || tempArray_vertices[i].m_vec != VertArray[curVert_Num])
		{
			VertArray.Add(tempArray_vertices[i].m_vec);
			curVert_Num++;
		}
		// set TriangleID table
		int TriNum = tempArray_vertices[i].mTriNum;
		int VNum = tempArray_vertices[i].mVertNum;
		pTriIDArray[TriNum].SetVertex(VNum, curVert_Num);
	}
	
	mp_solid->mVert_Num = curVert_Num+1;

	// temp CTriangle array is not used any more, delete it here
	delete []tempArray_vertices;

	return true;
}

// return number of triangles
int SolidSTL_Builder::Read_STL_File(CString &STLFileName, CTriangle *&pArray_Tri)
{
	if(mp_solid == NULL) return -1;
	FILE *fptr = NULL;
	fptr = fopen(STLFileName, "rb");
	if(fptr == NULL) return -1;
	//if (ftell(fp) == 0)	Read_stl_Binary(fp);
	//else Read_stl_Text(fp);

	// read the header
	fread(mp_solid->m_Header, 1, 80, fptr);
	// read Triangle number
	mp_solid->mTri_Num = 0;
	fread((char *)&mp_solid->mTri_Num, sizeof(int), 1, fptr);
	if(mp_solid->mTri_Num <= 0) return 0;

	// read each triangle
	if(pArray_Tri != NULL) delete[] pArray_Tri;
	pArray_Tri = new CTriangle[mp_solid->mTri_Num];
	for(int i=0;i<mp_solid->mTri_Num;i++)
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
		curT.ComputeCentroid();
		curT.CalcArea();
		curT.CalcSlope();
		curT.SetTag(attrib);

		pArray_Tri[i]=curT;
	}

	fclose(fptr);
	return mp_solid->mTri_Num;
}

// return the regular triangle number in pArray_Tri
int SolidSTL_Builder::RegulateTriangles(int TriNum, CTriangle *&pArray_Tri)
{
	int curTriNum = TriNum;
	for(int i=0;i<curTriNum;i++)
	{
		if( pArray_Tri[i][0] == pArray_Tri[i][1] ||
			pArray_Tri[i][0] == pArray_Tri[i][2] ||
			pArray_Tri[i][1] == pArray_Tri[i][2] )
		{
			CTriangle temp = pArray_Tri[i];
			pArray_Tri[i] = pArray_Tri[curTriNum-1];
			pArray_Tri[curTriNum-1]=temp;
			curTriNum--;	i--;
		}
	}
	return curTriNum;
}


