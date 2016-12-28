//////////////////////////////////////////////////////////////////////
// Author:		Yong CHEN
// Date:        12/05
// MeshTestbed - used for USC-ISE510
//////////////////////////////////////////////////////////////////////

#pragma once

enum Axis {AXIS_X=0, AXIS_Y, AXIS_Z};
#define M_PI 3.14159265358979323846264

#define FLOAT_EQ(f1, f2, t) (((f2 - t) < f1) && (f1 <( f2 + t)))

class CVertex  
{
public:
	static double tolerance; 
protected:
	int tag;			// storage for some property
	double vec[3];		// Storage for vector components
public:
	CVertex(void);
	CVertex(const double v[3]);
	CVertex(const double x, const double y, const double z);
	virtual ~CVertex(void);

	void operator =(const CVertex &v);
	CVertex &	setVec(const double v[3]);
	CVertex &	setVec(double x, double y, double z);
	void setTag(int atag);

	bool operator > (const CVertex &v);
	bool operator >=(const CVertex &v);
	bool operator < (const CVertex &v);
	bool operator <=(const CVertex &v);

	bool operator ==(const CVertex &rhs);
	bool operator !=(const CVertex &rhs);

	CVertex cross(const CVertex &v) const;
	double dot(const CVertex &v) const;

	const double *	getVec() const	{ return vec; }
	const int		getTag() const	{ return tag; }
	void	getValue(double &x, double &y, double &z) const;
	double &  operator [](int i) 	{ return (vec[i]); }
	const double & operator [](int i) const	{ return (vec[i]); }

	CVertex 	operator + (CVertex &v);
	CVertex &	operator +=(CVertex &v);
	CVertex 	operator - (CVertex &v);
	CVertex &	operator -=(CVertex &v);

	CVertex 	operator * (double d);
	CVertex &	operator *=(double d);
	CVertex 	operator / (double d);
	CVertex &	operator /=(double d);

	double	length() const;
	void	normalize();

	// some 3D geometry functions
	static bool Collinear(CVertex &v1, CVertex &v2, CVertex &v3);
	static bool Between(CVertex &v1, CVertex &v2, CVertex &test);
	static bool Within(CVertex &v1, CVertex &v2, CVertex &test);
	static bool ifEdgeContained(CVertex &v1a, CVertex &v1b, CVertex &v2a, CVertex &v2b);
	
	static int relation_Point_Line(CVertex &p, CVertex &La, CVertex &Lb, CVertex &N);
	static int relation_Point_Triangle(CVertex &p, CVertex &Ta, CVertex &Tb, CVertex &Tc);
	static int relation_Point_Tetrahedron(CVertex &p, CVertex &Ta, CVertex &Tb, CVertex &Tc, CVertex &Td);
	static int relation_Line_Triangle(CVertex &A, CVertex &B, CVertex &C, CVertex &La, CVertex &Lb);

	static double volume_Tetrahedron(CVertex &p, CVertex &A, CVertex &B, CVertex &C);
	static double volume_Tetraprism(CVertex &A, CVertex &B, CVertex &C,CVertex &AP, CVertex &BP, CVertex &CP);
};
