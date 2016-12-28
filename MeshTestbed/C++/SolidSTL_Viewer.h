//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#pragma once

class SolidSTL_Viewer
{
public:
	SolidSTL_Viewer(void);
	~SolidSTL_Viewer(void) {}

	// different drawing method: WireFrame, Points, Shaded, Transparent
	void DrawModel_WireFrame( SolidSTL *psolid );
	void DrawModel_WireFrameNoFlat( SolidSTL *psolid );
	// skip number of points for every point displayed
	void DrawModel_Points( SolidSTL *psolid, int ViewSkipVert );
	void DrawModel_Shaded( SolidSTL *psolid, float m_LightPos[4] );
	void DrawModel_Extent( SolidSTL *psolid );
	void DrawModel_WireFrameShaded( SolidSTL *psolid, float m_LightPos[4] );
	void DrawModel_Curvature( SolidSTL *psolid, float m_LightPos[4] );	

	void SelectModel( SolidSTL *psolid );

	void SetLightPos(float aLP[4]);
	void SetPartColor(float aPC[4]);

protected:
	void SetModelTransform( SolidSTL *psolid );

public:
	// Display attribute
	static float m_PartColor[4];
	static float m_AmbientLight[4];
	static float m_DiffuseLight[4];
};

