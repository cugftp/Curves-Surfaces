#pragma once
#include"spline.h"
class Curve :public Spline
{
public:
	Curve();
	bool ch;
//protected:
//	bool ch = false;
};
class BezierCurve :public Curve
{
public:
	BezierCurve(int i);
	void Paint(ArgParser * args);
	void OutputBezier(FILE *file);
	void OutputBSpline(FILE *file);
	TriangleMesh* OutputTriangles(ArgParser *args);
private:
	int bzpoints;	
protected:
	
};

class BSplineCurve :public Curve
{
public:
	BSplineCurve(int i);
	void Paint(ArgParser * args);
	void OutputBezier(FILE *file);
	void OutputBSpline(FILE *file);
private:
	int bspoints;
};
