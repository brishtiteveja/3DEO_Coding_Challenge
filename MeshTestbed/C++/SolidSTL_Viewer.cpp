//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <afxtempl.h>

#include "gl/gl.h"
#include "gl/glu.h"

#include ".\vertex.h"
#include ".\triangle.h"
#include ".\TransMatrix.h"
#include ".\solidstl.h"
#include ".\solidstl_viewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

float SolidSTL_Viewer::m_PartColor[4] = {0.0f, 1.0f, 1.0f, 1.0f };
float SolidSTL_Viewer::m_PartColorRed[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float SolidSTL_Viewer::m_PartColorGreen[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
float SolidSTL_Viewer::m_PartColorBlue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

float SolidSTL_Viewer::m_RegionColor[3][4] = { 
												{ 1.0f, 0.0f, 0.0f, 1.0f },
												{ 0.0f, 1.0f, 0.0f, 1.0f },
												{ 0.0f, 0.0f, 1.0f, 1.0f }
											 };

float SolidSTL_Viewer::m_AmbientLight[4] = { 0.35f, 0.35f, 0.35f, 1.0f };
float SolidSTL_Viewer::m_DiffuseLight[4] = { 0.35f, 0.35f, 0.35f, 1.0f };

SolidSTL_Viewer::SolidSTL_Viewer(void)
{
}

void SolidSTL_Viewer::SetPartColor(float aPC[4])
{
	for(int i=0; i<4; i++) m_PartColor[i] = aPC[i];
}

void SolidSTL_Viewer::SelectModel( SolidSTL *psolid )
{
	if(psolid == NULL) return;

	glLoadName(psolid->m_modelID);
}

void SolidSTL_Viewer::SetModelTransform( SolidSTL *psolid )
{
	if(psolid == NULL) return;

	// first combine the transformation of SolidSTL
	CTransMatrix modelMatrix;
	if(psolid->m_MatrixList.IsEmpty() == FALSE) 
		modelMatrix= psolid->m_MatrixList.GetTail();
	double M[16];
	for(int i=0; i<4; i++) 
	{
		for(int j=0; j<4; j++) 
		{
			M[i + j*4] = modelMatrix[i][j];
		}
	}
	glMultMatrixd(M); // apply local matrix;
}

void SolidSTL_Viewer::DrawModel_WireFrame( SolidSTL *psolid )
{
	if(psolid == NULL) return;
	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glEnable(GL_CULL_FACE);
	// display
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_LINE) ; 
	glPolygonMode (GL_BACK, GL_LINE) ; 
	glColor4fv(m_PartColor);
	for (int i=psolid->m_startDpyList; i <= psolid->m_endDpyList; i++)
	{
		glCallList(i);
	}

	glPopMatrix(); // restore the current view matrix
}

void SolidSTL_Viewer::DrawModel_WireFrameNoFlat( SolidSTL *psolid )
{
	if(psolid == NULL) return;
	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glEnable(GL_CULL_FACE);
	// display
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_LINE) ; 
	glPolygonMode (GL_BACK, GL_LINE) ; 
	//glColor4fv(m_PartColor);
	glLineWidth(1.0f);

	for(int i=0; i<psolid->HalfEdgeNum(); i++)
	{
		int iv1 = psolid->startV(i);
		int iv2 = psolid->endV(i);
		if(iv1 >= iv2) continue;	// avoid duplicate
		if(psolid->Hedge(i).Flat() == true) continue;

		CVertex v1 = psolid->Vert(iv1).Position();
		CVertex v2 = psolid->Vert(iv2).Position();

		glBegin(GL_LINES);
		int n = rand() % 4 + 0;
		switch (n) {
		case 0:
			glColor4fv(m_PartColor);
			break;
		case 1:
			glColor4fv(m_PartColorRed);
			break;
		case 2:
			glColor4fv(m_PartColorGreen);
			break;
		case 3:
			glColor4fv(m_PartColorBlue);
			break;
		default:
			glColor4fv(m_PartColor);
			break;
		}

			glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
			glVertex3f((float)v2[AXIS_X], (float)v2[AXIS_Y], (float)v2[AXIS_Z]);
		glEnd();			
	}		

	float pointColor[4] = {0.8f, 0.2f, 0.2f, 1.0f };
	glColor4fv(pointColor);
	glPointSize(10.0f);
	for(int i=0; i<psolid->mVert_Num; i++)
	{
		if(psolid->mVertArray[i].NonFlatValence() <= 6) continue;

		CVertex v1 = psolid->mVertArray[i].Position();
		glBegin(GL_POINTS);
			glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		glEnd();			
	}		

	glPopMatrix(); // restore the current view matrix
}

void SolidSTL_Viewer::DrawModel_Points( SolidSTL *psolid, int ViewSkipVert)
{
	if(psolid == NULL) return;
	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glDisable(GL_CULL_FACE);
	// display
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_LINE) ; 
	glPolygonMode (GL_BACK, GL_LINE) ; 

	glPointSize(3.0f);

	glColor4fv(m_PartColor);
	for(int ctr=0; ctr<psolid->mVert_Num; ctr=ctr + ViewSkipVert)
	{
		CVertex v1 = psolid->mVertArray[ctr].Position();
		glBegin(GL_POINTS);
			glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		glEnd();			
	}		

	glPopMatrix(); // restore the current view matrix
}
void SolidSTL_Viewer::DrawModel_WireFrameShaded( SolidSTL *psolid, float m_LightPos[4] )
{
	if(psolid == NULL) return;

	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glEnable(GL_CULL_FACE);
	// Set the light model
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_FILL) ; 
	glPolygonMode (GL_BACK, GL_FILL) ; 

	// display shade triangles
	GLfloat LocalLight0[4];
   	CTransMatrix IModelMatrix;
	if(psolid->m_MatrixList.IsEmpty() == FALSE) 
		IModelMatrix = psolid->m_MatrixList.GetTail();
	IModelMatrix.Inverse();
	IModelMatrix.GetTransformedHVertex(m_LightPos, LocalLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, LocalLight0);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColor);
	for (int i=psolid->m_startDpyList; i <= psolid->m_endDpyList; i++)
	{
		/*
		int n = rand() % 4 + 0;
		switch (n) {
		case 0:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColor);
			break;
		case 1:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColorRed);
			break;
		case 2:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColorGreen);
			break;
		case 3:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColorBlue);
			break;
		default:
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColor);
			break;
		}
		*/
		glCallList(i);
	}

	// display wireframe
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_LINE) ; 
	glPolygonMode (GL_BACK, GL_LINE) ; 
	glLineWidth(2.0f);
	glColor4fv(m_PartColor);
	for (int i=psolid->m_startDpyList; i <= psolid->m_endDpyList; i++)
	{
		glCallList(i);
	}
	glLineWidth(1.0f);

	glPopMatrix(); // restore the current view matrix
}

bool SolidSTL_Viewer::Build_CTL_Color(SolidSTL* mp_solid) {
	for (int i = 0; i < mp_solid->mTri_Num; i++)
	{
		// Determining slope color
		CVertex Norm = mp_solid->mTriArray[i].Normal();
		double Norm_z = Norm.getVec()[2];

		// Current triangle slope and determine its color
		int colorID;
		if (Norm_z > 0) { // Up slope  ,   Red Color
			colorID = 0;
		}
		else if (Norm_z < 0) { // Down slope   ,  Green Color
			colorID = 1;
		}
		else {  // Vertical    ,   Blue Color
			colorID = 2;
		}

		mp_solid->mTriArray[i].SetColorID(colorID);
	}

	return true;
}

bool SolidSTL_Viewer::Build_CTL_Color_Logic2(SolidSTL* mp_solid) {
	//determining slice no
	double minZ = INFINITY;
	double maxZ = -INFINITY;
	for (int i = 0; i < mp_solid->mTri_Num; i++)
	{
		CVertex v1 = mp_solid->mVertArray[mp_solid->mTriArray[i][0]].Position();
		CVertex v2 = mp_solid->mVertArray[mp_solid->mTriArray[i][1]].Position();
		CVertex v3 = mp_solid->mVertArray[mp_solid->mTriArray[i][2]].Position();

		// Getting slice ID for each vertex of the triangle
		double v1Z = v1.getVec()[2];
		double v2Z = v2.getVec()[2];
		double v3Z = v3.getVec()[2];

		int sliceID1 = (int)((v1Z - minZ) / SLICE_THICKNESS);
		int sliceID2 = (int)((v2Z - minZ) / SLICE_THICKNESS);
		int sliceID3 = (int)((v3Z - minZ) / SLICE_THICKNESS);
		int minSliceID = min(sliceID1, min(sliceID2, sliceID3));
		int maxSliceID = max(sliceID1, max(sliceID2, sliceID3));

		mp_solid->mTriArray[i].SetMinSliceID(minSliceID); // Lowest slice that includes the triangle
		mp_solid->mTriArray[i].SetMaxSliceID(maxSliceID); // Highest slice that includes the triangle

		double minZT = min(v1Z, min(v2Z, v3Z));
		double maxZT = max(v1Z, max(v2Z, v3Z));

		if (maxZT > maxZ)
			maxZ = maxZT;
		if (minZT < minZ)
			minZ = minZT;
	}

	// Determining number of slices
	int MAX_SLICE_NO = (int)((maxZ - minZ) / SLICE_THICKNESS);

	// storage for slice color
	int *mSliceColorID = new int[MAX_SLICE_NO]; 
	fill_n(mSliceColorID, MAX_SLICE_NO, -1);  // initialize slice color id

	for (int i = 0; i < mp_solid->mTri_Num; i++)
	{
		int minSliceID = mp_solid->mTriArray[i].MinSliceID();
		int maxSliceID = mp_solid->mTriArray[i].MaxSliceID();

		// Determining slope color
		CVertex Norm = mp_solid->mTriArray[i].Normal();
		double Norm_z = Norm.getVec()[2];

		// Current triangle slope and determine its color
		int colorID;
		if (Norm_z > 0) { // Up slope  ,   Red Color
			colorID = 0;
		}
		else if (Norm_z < 0) { // Down slope   ,  Green Color
			colorID = 1;
		}
		else {  // Vertical    ,   Blue Color
			colorID = 2;
		}

		// setting color id for all slices in between the triangle
		// Check whether other slices are already colored
		// Checking slice color and updating with priority DOWN > UP > 2.5D
		int sliceColorID = mSliceColorID[0];
		for (int k = minSliceID; k < maxSliceID; k++) {
			if (sliceColorID == 1 || colorID == 1) {    // triangle in downslope, if any triangle has downslope in the slice, all triangle will have downslope
				sliceColorID = colorID = 1;
				mSliceColorID[k] = 1;
			}
			else if (sliceColorID == 0 || colorID == 0) { // if this triangle or any any other triangle in the slice in up slope
				sliceColorID = colorID = 0;
				mSliceColorID[k] = 0;
			}
			else if (sliceColorID == 2 || colorID == 2) {// Vertical
				sliceColorID = colorID = 2;
				mSliceColorID[k] = 2;
			}
			else {	// slice not yet colored
				sliceColorID = colorID;
				mSliceColorID[k] = colorID;
			}
		}
		
		mp_solid->mTriArray[i].SetColorID(sliceColorID);

	}

	return true;
}

bool SolidSTL_Viewer::Build_CTL_Color_Logic3(SolidSTL* psolid) {
	//determining slice no
	double min = INFINITY;
	double max = -INFINITY;
	for (int i = 0; i < psolid->mTri_Num; i++)
	{
		CVertex Centroid = psolid->mTriArray[i].Centroid();
		double triZ = Centroid.getVec()[2];
		if (triZ > max)
			max = triZ;
		if (triZ < min)
			min = triZ;
	}

	int MAX_SLICE_NO = (int)((max-min) / SLICE_THICKNESS);
	
	float mSliceColor[4];
	char str[256];

	int *mSliceColorID = new int[MAX_SLICE_NO];
	fill_n(mSliceColorID, MAX_SLICE_NO, -1);  // initialize slice color id

	for (int i = 0; i < psolid->mTri_Num; i++)
	{
		CVertex v1 = psolid->mVertArray[psolid->mTriArray[i][0]].Position();
		CVertex v2 = psolid->mVertArray[psolid->mTriArray[i][1]].Position();
		CVertex v3 = psolid->mVertArray[psolid->mTriArray[i][2]].Position();
		//v1.getVec()[3]
		CVertex Norm = psolid->mTriArray[i].Normal();

		CVertex Centroid = psolid->mTriArray[i].Centroid();
		double triZ = Centroid.getVec()[2];
		int sliceID = (int)((triZ - min) / SLICE_THICKNESS);

		double v1_z = v1.getVec()[2];
		double v2_z = v2.getVec()[2];
		double v3_z = v3.getVec()[2];

		int sliceID1 = (int)((v1_z - min) / SLICE_THICKNESS);
		int sliceID2 = (int)((v2_z - min) / SLICE_THICKNESS);
		int sliceID3 = (int)((v3_z - min) / SLICE_THICKNESS);

		sliceID = (sliceID1 + sliceID2 + sliceID3) / 3;
		//sprintf_s(str, "sid1 = %d sid2 = %d sid3 = %d sid = %d \n", sliceID1, sliceID2, sliceID3, sliceID);
		OutputDebugString(str);

		psolid->mTriArray[i].SetMinSliceID(sliceID);

		double Norm_z = Norm.getVec()[2];

		int colorID;
		if (Norm_z > 0) { // Up slope  ,   Red Color
			colorID = 0;
		}
		else if (Norm_z < 0) { // Down slope   ,  Green Color
			colorID = 1;
		}
		else {  // Vertical    ,   Blue Color
			colorID = 2;
		}

		psolid->mTriArray[i].SetColorID(colorID);

		/*
		int sliceColorID = mSliceColorID[sliceID];
		if (sliceColorID != -1) {
			mSliceColorID[sliceID] = sliceColorID = colorID;
			psolid->mTriArray[i].SetColorID(colorID);
		}
		else {
			if (sliceColorID == 1 || colorID == 1)    // triangle in downslope, if any triangle has downslope in the slice, all triangle will have downslope
				 psolid->mTriArray[i].SetColorID(1);
			else if (sliceColorID == 0 || colorID == 0) // triangle in up slope
				 psolid->mTriArray[i].SetColorID(0);
			else
				 psolid->mTriArray[i].SetColorID(0); // triangle in 2.5 D
			}
		*/
	}
	return true;
}


void SolidSTL_Viewer::DrawModel_ColorSTL(SolidSTL *psolid, float m_LightPos[4])
{
	if (psolid == NULL) return;
	if (psolid->ColorBuilt() == false) {
		// logic 1
		//Build_CTL_Color(psolid);
		// logic 2
		// Build_CTL_Color_Logic2(psolid);
		// logic 3
		Build_CTL_Color_Logic3(psolid);
		psolid->SetColorBuilt(true);
	}


	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform(psolid);

	glEnable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	// Set the light model
	//glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	//glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_SMOOTH);

	// display
	//GLfloat LocalLight0[4];
	//  	CTransMatrix IModelMatrix;
	//if(psolid->m_MatrixList.GetSize() > 0) 
	//	IModelMatrix = psolid->m_MatrixList.GetTail();
	//IModelMatrix.Inverse();
	//IModelMatrix.GetTransformedHVertex(m_LightPos, LocalLight0);
	//glLightfv(GL_LIGHT0, GL_POSITION, LocalLight0);
	float CurvColor[3][4] =
	{ { 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	glBegin(GL_TRIANGLES);


	for (int i = 0; i<psolid->mTri_Num; i++)
	{
		CVertex v1 = psolid->mVertArray[psolid->mTriArray[i][0]].Position();
		CVertex v2 = psolid->mVertArray[psolid->mTriArray[i][1]].Position();
		CVertex v3 = psolid->mVertArray[psolid->mTriArray[i][2]].Position();
		//v1.getVec()[3]
		CVertex Norm = psolid->mTriArray[i].Normal();

		glNormal3f((float)Norm[AXIS_X], (float)Norm[AXIS_Y], (float)Norm[AXIS_Z]);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[0]);
		
		int colorID = psolid->mTriArray[i].ColorID();
		glColor4fv(m_RegionColor[colorID]);

		glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		//glColor3f(m_RegionColor[1][0], m_RegionColor[1][1], m_RegionColor[1][2]);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[1]);
		//glColor3f(0.0, 0.0, 1.0);
		glVertex3f((float)v2[AXIS_X], (float)v2[AXIS_Y], (float)v2[AXIS_Z]);
		//glColor3f(m_RegionColor[2][0], m_RegionColor[2][1], m_RegionColor[2][2]);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[2]);
		//glColor3f(0.0, 1.0, 0.0);
		glVertex3f((float)v3[AXIS_X], (float)v3[AXIS_Y], (float)v3[AXIS_Z]);
	}
	glEnd();

	glPopMatrix(); // restore the current view matrix

	glDisable(GL_BLEND);
	//glDisable(GL_LINE_SMOOTH);
	//glDisable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_FLAT);
}


void SolidSTL_Viewer::DrawModel_Shaded( SolidSTL *psolid, float m_LightPos[4] )
{
	if(psolid == NULL) return;

	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glEnable(GL_CULL_FACE);
	// Set the light model
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_FILL) ; 
	glPolygonMode (GL_BACK, GL_FILL) ; 

	// display
	GLfloat LocalLight0[4];
   	CTransMatrix IModelMatrix;
	if(psolid->m_MatrixList.IsEmpty() == FALSE) 
		IModelMatrix = psolid->m_MatrixList.GetTail();
	IModelMatrix.Inverse();
	IModelMatrix.GetTransformedHVertex(m_LightPos, LocalLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, LocalLight0);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_PartColor);
	for (int i=psolid->m_startDpyList; i <= psolid->m_endDpyList; i++)
	{
		glCallList(i);
	}

	glPopMatrix(); // restore the current view matrix
}

void SolidSTL_Viewer::DrawModel_Extent( SolidSTL *psolid )
{
	if(psolid == NULL) return;
	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	// display
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPolygonMode (GL_FRONT, GL_LINE) ; 
	glPolygonMode (GL_BACK, GL_LINE) ; 
	glColor4fv(m_PartColor);

	glBegin(GL_LINES);
	// line 1
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 2
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	// line 3
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	// line 4
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	// line 5
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 6
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 7
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 8
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 9
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 10
	glVertex3f((float)psolid->m_PartMin[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	// line 11
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMax[AXIS_Z]);
	// line 12
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMin[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glVertex3f((float)psolid->m_PartMax[AXIS_X], (float)psolid->m_PartMax[AXIS_Y], (float)psolid->m_PartMin[AXIS_Z]);
	glEnd();

	glPopMatrix(); // restore the current view matrix
}

void SolidSTL_Viewer::DrawModel_Curvature( SolidSTL *psolid, float m_LightPos[4] )
{
	if(psolid == NULL) return;

	// first combine the transformation of SolidSTL
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // store the current view matrix
	SetModelTransform( psolid );

	glEnable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	// Set the light model
	//glLightfv(GL_LIGHT0, GL_AMBIENT, m_AmbientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, m_DiffuseLight);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	glPolygonMode (GL_FRONT, GL_FILL) ; 
	glPolygonMode (GL_BACK, GL_FILL) ; 
	//glEnable(GL_BLEND);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_SMOOTH);

	// display
	//GLfloat LocalLight0[4];
 //  	CTransMatrix IModelMatrix;
	//if(psolid->m_MatrixList.GetSize() > 0) 
	//	IModelMatrix = psolid->m_MatrixList.GetTail();
	//IModelMatrix.Inverse();
	//IModelMatrix.GetTransformedHVertex(m_LightPos, LocalLight0);
	//glLightfv(GL_LIGHT0, GL_POSITION, LocalLight0);
	float CurvColor[3][4] =
	{	{ 1.0f, 0.0f, 0.0f, 1.0f },	
		{ 0.0f, 1.0f, 0.0f, 1.0f },	
		{ 0.0f, 0.0f, 1.0f, 1.0f } 
	};

	glBegin(GL_TRIANGLES);
	for(int i=0; i<psolid->mTri_Num; i++)
	{
		CVertex v1 = psolid->mVertArray[psolid->mTriArray[i][0]].Position();
		CVertex v2 = psolid->mVertArray[psolid->mTriArray[i][1]].Position();
		CVertex v3 = psolid->mVertArray[psolid->mTriArray[i][2]].Position();
		CVertex Norm = psolid->mTriArray[i].Normal();

		glNormal3f((float)Norm[AXIS_X], (float)Norm[AXIS_Y], (float)Norm[AXIS_Z]);
		//glColor3f(1.0, 0.0, 0.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[0]);
		glVertex3f((float)v1[AXIS_X], (float)v1[AXIS_Y], (float)v1[AXIS_Z]);
		//glColor3f(0.0, 0.0, 1.0);
		glColor3f(m_RegionColor[1][0], m_RegionColor[1][1], m_RegionColor[1][2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[1]);
		glVertex3f((float)v2[AXIS_X], (float)v2[AXIS_Y], (float)v2[AXIS_Z]);
		//glColor3f(0.0, 1.0, 0.0);
		glColor3f(m_RegionColor[2][0], m_RegionColor[2][1], m_RegionColor[2][2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_RegionColor[2]);
		glVertex3f((float)v3[AXIS_X], (float)v3[AXIS_Y], (float)v3[AXIS_Z]);
	}	
	glEnd();

	glPopMatrix(); // restore the current view matrix
	
	glDisable(GL_BLEND);
	//glDisable(GL_LINE_SMOOTH);
	//glDisable(GL_POLYGON_SMOOTH);
	glShadeModel(GL_FLAT);
}

