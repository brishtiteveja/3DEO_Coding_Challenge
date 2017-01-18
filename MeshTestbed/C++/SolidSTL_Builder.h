//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#pragma once

// serve to get rid of vertex redundant in Build_CTL.
class CSortVertex
{
public:
	CVertex m_vec;
	int mTriNum;
	int mVertNum;

	CSortVertex(CVertex &avec, int aTriNum, int aVertNum);
	CSortVertex();
	CSortVertex(const CSortVertex &asv);
	~CSortVertex() {}
	void operator =(const CSortVertex &asv);

	static void quick_sort_vertex_array(CSortVertex *tempArray_vertices, int low, int high);
	static void exchange_vertex(CSortVertex *tempArray_vertices, int v1, int v2);
	static int compare_vertex(CSortVertex *tempArray_vertices, int v1, int v2);
	static int compare_vertex_with_tolerance(CSortVertex *tempArray_vertices, int v1, int v2);
};

// serve to build half-edge tables.
class CSortEdge
{
public:
	int mv1, mv2;
	int mENum; 

	CSortEdge(int v1, int v2, int ENum );
	CSortEdge();
	CSortEdge(const CSortEdge &asv);
	~CSortEdge() {}
	void operator =(const CSortEdge &asv);

	static void quick_sort_edge_array(CSortEdge *tempArray_edges, int low, int high);
	static void exchange_edge(CSortEdge *tempArray_edges, int v1, int v2);
	static int compare_edge(CSortEdge *tempArray_edges, int v1, int v2);
};
	// sort the edge array, build the r array	
	// OUTPUT:	using a temporay edge table to sort the half edges,
	//			build the r array according to the ajacency information
	//			in the sorted array	
	// RETURN:	none
	int sort_edge_array();
	void quick_sort_edge_array(int low, int hight);
	void exchange_edge(int e1, int e2);
	// compare e with ce, if e.v1 < ce.v1, return -1;
	// if e.v1 == ce.v1 && e.v2 < ce.v2, return -1;
	// if e.v1 == ce.v1 && e.v2 == ce.v2, return 0;
	// otherwise return 1.
	int compare_edge(int e1, int e2);


// Construct a STL_Solid from a given STL file.
class SolidSTL_Builder
{
public:
	SolidSTL_Builder(void);
	~SolidSTL_Builder(void);

	// build complete information for SolidSTL
	bool Construct(CString &STLFileName, SolidSTL *ap_Solid, bool bSolidSTL, CProgressCtrl *aProgressCtrl=NULL, CDemoInfo *aDemoInfo=NULL);
	bool Construct(int TriNum, CTriangle *pArray_Tri, SolidSTL *ap_Solid, CProgressCtrl *aProgressCtrl=NULL, CDemoInfo *aDemoInfo=NULL);

protected:
	// step 1
	int Read_STL_File(CString &STLFileName, CTriangle *&pArray_Tri);
	int RegulateTriangles(int TriNum, CTriangle *&pArray_Tri);
	// step 2
	bool Build_CTL(CTriangle *&pArray_Tri, 
		CArray <CVertex, CVertex&> &VertArray, CTriangleID *pTriIDArray);
	// step 3
	bool Build_Topology();
	// step 4
	int CheckShellInfo();
	void propagate_shell_triangle(int root, int aShell);
	// step 5
	void CalcHedgeInfo();
	// step 6
	void CalcVertexInfo();
	void propagate_triangle_vertex(int root);

	// data
	SolidSTL *mp_solid;
};
