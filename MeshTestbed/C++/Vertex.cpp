//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <math.h>
#include <assert.h>

#include ".\vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double CVertex::tolerance = 1.0e-5; 

CVertex::CVertex(void)
{
	tag = 0;
	vec[0] = vec[1] = vec[2] = 0.0;
}

CVertex::CVertex(const double v[3])
{ 
	tag = 0;
	vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; 
}

CVertex::CVertex(const double x, const double y, const double z)
{ 
	tag = 0;
	vec[0] = x; vec[1] = y; vec[2] = z; 
}

CVertex::~CVertex(void)
{
	// do nothing
}

void CVertex::operator =(const CVertex &v)
{
	vec[0] = v.vec[0];
	vec[1] = v.vec[1];
	vec[2] = v.vec[2];	  
	tag = v.tag;
}

CVertex& CVertex::setVec(const double v[3])
{ 
	vec[0] = v[0]; 
	vec[1] = v[1]; 
	vec[2] = v[2]; 
	return *this; 
}
 
CVertex& CVertex::setVec(double x, double y, double z)
{ 
	vec[0] = x; 
	vec[1] = y; 
	vec[2] = z; 
	return *this; 
}

void CVertex::setTag(int atag)
{ 
	tag = atag;
}

bool CVertex::operator >(const CVertex &v)  // no tolerance is used
{
	if(vec[0] > v.vec[0]) return true;
	if(vec[0] < v.vec[0]) return false;
	
	if(vec[1] > v.vec[1]) return true;
	if(vec[1] < v.vec[1]) return false;
	
	if(vec[2] > v.vec[2]) return true;
	if(vec[2] < v.vec[2]) return false;
	
	return false;
}

bool CVertex::operator >=(const CVertex &v)	// judging with tolerance
{
	if(vec[0] > v.vec[0] + tolerance)	return true;
	if(vec[0] < v.vec[0] - tolerance)	return false;
	
	if(vec[1] > v.vec[1] + tolerance)	return true;
	if(vec[1] < v.vec[1] - tolerance)	return false;

	if(vec[2] > v.vec[2] + tolerance)	return true;
	if(vec[2] < v.vec[2] - tolerance)	return false;
	
	return false;
}

bool CVertex::operator <(const CVertex &v)
{
	if(vec[0] > v.vec[0])	return false;
	if(vec[0] < v.vec[0])	return true;
	
	if(vec[1] > v.vec[1])	return false;
	if(vec[1] < v.vec[1])	return true;
	
	if(vec[2] > v.vec[2])	return false;
	if(vec[2] < v.vec[2])	return true;
	
	return false;
}

bool CVertex::operator <=(const CVertex &v)
{
	if(vec[0] > v.vec[0] + tolerance)	return false;
	if(vec[0] < v.vec[0] - tolerance)	return true;
	
	if(vec[1] > v.vec[1] + tolerance)	return false;
	if(vec[1] < v.vec[1] - tolerance)	return true;
	
	if(vec[2] > v.vec[2] + tolerance)	return false;
	if(vec[2] < v.vec[2] - tolerance)	return true;
	
	return false;
}

bool CVertex::operator ==(const CVertex &rhs)
{
	if( fabs(vec[0] - rhs.vec[0]) < tolerance &&
		fabs(vec[1] - rhs.vec[1]) < tolerance &&
		fabs(vec[2] - rhs.vec[2]) < tolerance)
		return true;
	else
		return false;
}

bool CVertex::operator !=(const CVertex &rhs)
{
	return !(*this == rhs);
}

CVertex CVertex::cross(const CVertex &v) const
{
	return CVertex( vec[AXIS_Y]*v[AXIS_Z] - vec[AXIS_Z]*v[AXIS_Y], 
					vec[AXIS_Z]*v[AXIS_X] - vec[AXIS_X]*v[AXIS_Z], 
					vec[AXIS_X]*v[AXIS_Y] - vec[AXIS_Y]*v[AXIS_X]);
}

double CVertex::dot(const CVertex &v) const
{
	return (vec[AXIS_X]*v[AXIS_X] + vec[AXIS_Y]*v[AXIS_Y] + vec[AXIS_Z]*v[AXIS_Z]) ;
}

void CVertex::getValue(double &x, double &y, double &z) const
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

CVertex& CVertex::operator +=(CVertex &v)
{
	vec[0] += v[0];
	vec[1] += v[1];
	vec[2] += v[2];
	return *this;
}

CVertex CVertex::operator +(CVertex &v)
{
	CVertex resultV(*this);
	resultV += v;
	return resultV;
}

CVertex& CVertex::operator -=(CVertex &v)
{
	vec[0] -= v[0];
	vec[1] -= v[1];
	vec[2] -= v[2];
	return *this;
}

CVertex CVertex::operator -(CVertex &v)
{
	CVertex resultV(*this);
	resultV -= v;
	return resultV;
}

CVertex CVertex::operator *(double d)
{
	CVertex resultV(*this);
	resultV *= d;
	return resultV;
}

CVertex& CVertex::operator *=(double d)
{
	vec[0] *= d;
	vec[1] *= d;
	vec[2] *= d;
	return *this;
}

CVertex CVertex::operator /(double d)
{
	CVertex resultV(*this);
	resultV /= d;
	return resultV;
}

CVertex& CVertex::operator /=(double d)
{
	vec[0] /= d;
	vec[1] /= d;
	vec[2] /= d;
	return *this;
}

double CVertex::length() const
{
	return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

void CVertex::normalize()
{
	double dist = length();
	if(dist > tolerance)
		*this /= dist;	
}

/*********************** 3D Geometry **********************************/
bool CVertex::Collinear(CVertex &v1, CVertex &v2, CVertex &v3)
{
	return
	   (fabs(( v3[2] - v1[2] ) * (v2[1] - v1[1]) -
			 ( v2[2] - v1[2] ) * (v3[1] - v1[1])) < tolerance
	&&	fabs(( v2[2] - v1[2] ) * (v3[0] - v1[0]) -
			 ( v2[0] - v1[0] ) * (v3[2] - v1[2])) < tolerance
	&&	fabs(( v2[0] - v1[0] ) * (v3[1] - v1[1]) -
			 ( v2[1] - v1[1] ) * (v3[0] - v1[0])) < tolerance);
}

// return true if test is between [v1, v2]
bool CVertex::Between(CVertex &v1, CVertex &v2, CVertex &test)
{
	if(! Collinear(v1, v2, test))
		return false;

	// if v1v2 not vertical, check betweenness on x; else on  y; else on z
	if( fabs(v1[0] - v2[0])>tolerance )
		return ((v1[0] <= test[0]+tolerance && test[0] <= v2[0]+tolerance) ||
				(v1[0]+tolerance >= test[0] && test[0]+tolerance >= v2[0]));
	else if( fabs(v1[1] - v2[1])>tolerance )
		return ((v1[1] <= test[1]+tolerance && test[1] <= v2[1]+tolerance) ||
				(v1[1]+tolerance >= test[1] && test[1]+tolerance >= v2[1]));
	else // judge in z
		return ((v1[2] <= test[2]+tolerance && test[2] <= v2[2]+tolerance) ||
				(v1[2]+tolerance >= test[2] && test[2]+tolerance >= v2[2]));
}			

// return true if test is between (v1, v2)
bool CVertex::Within(CVertex &v1, CVertex &v2, CVertex &test)
{
	if(! Collinear(v1, v2, test))
		return false;

	// if v1v2 not vertical, check betweenness on x; else on  y; else on z
	if( fabs(v1[0] - v2[0])>tolerance )
		return ((v1[0]+tolerance < test[0] && test[0]+tolerance < v2[0]) ||
				(v1[0] > test[0]+tolerance && test[0] > v2[0]+tolerance));
	else if( fabs(v1[1] - v2[1])>tolerance )
		return ((v1[1]+tolerance < test[1] && test[1]+tolerance < v2[1]) ||
				(v1[1] > test[1]+tolerance && test[1] > v2[1]+tolerance));
	else // judge in z
		return ((v1[2]+tolerance < test[2] && test[2]+tolerance < v2[2]) ||
				(v1[2] > test[2]+tolerance && test[2] > v2[2]+tolerance));
}			

bool CVertex::ifEdgeContained(CVertex &v1a, CVertex &v1b, CVertex &v2a, CVertex &v2b)
{
	if( Collinear(v1a, v1b, v2a) == false ||
		Collinear(v1a, v1b, v2b) == false)
		return false;
	
	if( Within(v1a, v1b, v2a) == true || 
		Within(v1a, v1b, v2b) == true ||
		Within(v2a, v2b, v1a) == true ||
		Within(v2a, v2b, v1b) == true )
		return true;

	if( Between(v1a, v1b, v2a) == true &&
		Between(v1a, v1b, v2b) == true &&
		Between(v2a, v2b, v1a) == true &&
		Between(v2a, v2b, v1b) == true )
		return true;
	else
		return false;
}

// if p lies to the left of the oriented line AB (related to plane 
//	normal N), return 1;
// if p lies to the right of the line AB (related to plane normal N), 
//	return -1;
// if p lies on the line AB, return 0.
int CVertex::relation_Point_Line(CVertex &p, CVertex &La, CVertex &Lb, CVertex &N)
{	
	// the signed volume is AP*(NxAB)
	CVertex v1 = La - p;
	CVertex v2 = La - Lb;
	CVertex temp = N.cross(v2);
	double volume = v1.dot(temp);

	if(volume > tolerance) return 1;
	else if(volume < -tolerance) return -1;
	else return 0;
}

// if p lies to the left of the oriented triangle ABC, return 1;
// if p lies to the right of the oriented triangle ABC, return -1;
// if p lies on the triangle, return 0.
int CVertex::relation_Point_Triangle(CVertex &p, CVertex &Ta, CVertex &Tb, CVertex &Tc)
{	
	double volume = volume_Tetrahedron(Ta, Tb, Tc, p);

	if(volume > tolerance) return 1;		// p lies to the left of the oriented plane
	else if(volume < -tolerance) return -1;	// p lies to the right
	else return 0;							// p lies on the plane
}

// the tetrahedron T is formed by (A, B, C and D)
// return 1 if p is inside T
// return -1 if p is outside T
// return 0 if p lies on a triangle of T
// return 2 if p lies on edge of T
// return 3 if p lies on vertex of T
// return -2 if triangle is degenerate, A=B=C=D=P
int CVertex::relation_Point_Tetrahedron(CVertex &p, 
										CVertex &Ta, CVertex &Tb, 
										CVertex &Tc, CVertex &Td)
{	
	int Rabc, Rbdc, Racd, Radb;
	Rabc = relation_Point_Triangle(p, Ta, Tb, Tc);
	Rbdc = relation_Point_Triangle(p, Tb, Td, Tc);
	Racd = relation_Point_Triangle(p, Ta, Tc, Td);
	Radb = relation_Point_Triangle(p, Ta, Td, Tb);
	
	if(Rabc + Rbdc + Racd + Radb == 4) // P lies on the left of all three
		return 1;	// p lies inside the triangle and ABC is counterclockwise
	else if(Rabc + Rbdc + Racd + Radb == -4)// P lies on the right of all three
		return 1;	// p lies inside the triangle and ABC is clockwise
	else if((Rabc == 1 && Rbdc == -1) || (Rabc == 1 && Racd == -1) ||
			(Rabc == 1 && Radb == -1) || (Rabc == -1 && Rbdc == 1) || 
			(Rabc == -1 && Racd == 1) ||(Rabc == -1 && Radb == 1)  ||
			(Rbdc == 1 && Racd == -1) || (Rbdc == 1 && Radb == -1) ||
			(Rbdc == -1 && Racd == 1) || (Rbdc == -1 && Radb == 1) ||
			(Racd == 1 && Radb == -1) ||(Racd == -1 && Radb == 1)) 
	// P lies on the left of one triangle and on the right of another
		return -1;	// p lies outside of the tetrahedron
	else if((Rabc == 0 && Rbdc != 0 && Racd != 0 && Radb !=0) ||			
			(Rbdc == 0 && Rabc != 0 && Racd != 0 && Radb !=0) ||
			(Racd == 0 && Rabc != 0 && Rbdc != 0 && Radb !=0) ||
			(Radb == 0 && Rabc != 0 && Rbdc != 0 && Racd !=0))
	// p lies on one triangle and is either on the left of the other two 
	// or on the right of the other three
		return 0;	// p lies on an triangle
	else if((Rabc == 0 && Rbdc == 0 && Racd != 0 && Radb != 0) ||
			(Rabc == 0 && Rbdc != 0 && Racd == 0 && Radb != 0) ||
			(Rabc == 0 && Rbdc != 0 && Racd != 0 && Radb == 0) ||
			(Rabc != 0 && Rbdc != 0 && Racd == 0 && Radb == 0) ||
			(Rabc != 0 && Rbdc == 0 && Racd != 0 && Radb == 0) ||
			(Rabc != 0 && Rbdc == 0 && Racd == 0 && Radb != 0))
			return 2;	// p lies on an edge of tetrahedron
	else if((Rabc == 0 && Rbdc == 0 && Racd == 0 && Radb != 0) ||
			(Rabc == 0 && Rbdc == 0 && Racd != 0 && Radb == 0) ||			
			(Rabc == 0 && Rbdc != 0 && Racd == 0 && Radb == 0) ||
			(Rabc != 0 && Rbdc == 0 && Racd == 0 && Radb == 0))	
			return 3;	// p is a vertex of the triangle
	else
	{
		assert(Rabc == 0 && Rbdc == 0 && Racd == 0 && Radb == 0);
		return -2;	// p lies on all four triangles, the tetrahedron is degenerate
	}
}

// INPUT:	A, B, C -- 3 vertice of a triangle
//			La, Lb -- 2 vertices of a line
// OUTPUT:	construct a tetrahedron with ABC-La, and 3 other
//			tetrahedra Lb-CBLa, Lb-ACLa, Lb-BALa. if the signed volumes of
//			the 4 tetrahera have the sign, (La, Lb) hits triangle ABC
// RETURN:	1 -- hit, 0 -- no hit, -1 -- all in one plane
int CVertex::relation_Line_Triangle(CVertex &A, CVertex &B, CVertex &C, 
									CVertex &La, CVertex &Lb)
{
	double vol1, vol2, vol3, vol4;
	vol1 = volume_Tetrahedron(A, B, C, La);
	vol2 = volume_Tetrahedron(Lb, A, C, La);
	vol3 = volume_Tetrahedron(Lb, C, B, La);
	vol4 = volume_Tetrahedron(Lb, B, A, La);

	if( fabs(vol2) < tolerance || fabs(vol3) < tolerance || fabs(vol4) < tolerance ) 
		return -1;
	if(vol1<=0.0 && vol2<0.0 && vol3<0.0 && vol4<0.0)
		return 1;
	if(vol1>=0.0 && vol2>0.0 && vol3>0.0 && vol4>0.0)
		return 1;
	return 0;
}

// INPUT:	p, A, B, C -- 4 vertice of a tetrahedron
// OUTPUT:	calculate the signed volume of p-ABC. The sign (+/-) of the
//			volume indicates the normal direction of ABC
// RETURN:	signed volume of tetrahedron p-ABC
double CVertex::volume_Tetrahedron(CVertex &p, CVertex &A, CVertex &B, CVertex &C)
{
	// the signed volume of tetrahedron (A, B, C, D) is AC*(BCxCD)/6
	CVertex v1 = A-p;
	CVertex v2 = B-p;
	CVertex v3 = C-p;

	CVertex temp = v1.cross(v2);
	return (temp.dot(v3)/6.0);
}

// INPUT:	A, B, C -- 3 vertice of the top face of a tetra-prism
//			A', B', C' - 3 related vertices of A, B, C
// OUTPUT:	calculate the signed volume of ABC-C'B'A'. The sign (+/-) of 
//			the	volume indicates the normal direction of ABC
// RETURN:	signed volume of tetra-prism ABC-C'B'A'
double CVertex::volume_Tetraprism(CVertex &A, CVertex &B, CVertex &C,
						CVertex &AP, CVertex &BP, CVertex &CP)
{
	// A tetraprism is the union of three tetrahedra
	// that is B-C'B'A', C-A'C'B, A-A'CB
	double th1, th2, th3;
	th1 = volume_Tetrahedron(B, CP, BP, AP);
	th2 = volume_Tetrahedron(C, AP, CP, B);
	th3 = volume_Tetrahedron(A, AP, C, B);

	// add the volumes together
	return (th1 + th2 + th3);
}
