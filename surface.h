#pragma once
#include"spline.h"
#include"curve.h"
class Surface :public Spline
{

};

class SurfaceOfRevolution :public Surface
{
public:
	SurfaceOfRevolution(Curve*c);
	void Paint(ArgParser * args);
	TriangleMesh* OutputTriangles(ArgParser *args);
	int getNumVertices();
    Vec3f getVertex(int i);
	void moveControlPoint(int selectedPoint, float x, float y);
	void addControlPoint(int selectedPoint, float x, float y);
	void deleteControlPoint(int selectedPoint);
private:
	bool ch1;
	Curve *C;
};

class BezierPatch :public Surface
{
public:
	BezierPatch();
	void Paint(ArgParser * args);
	void set(int i, Vec3f v);
	int getNumVertices();
	Vec3f getVertex(int i);
	TriangleMesh* OutputTriangles(ArgParser *args);
	void SetCurveVertex(ArgParser* args);
private:
	int points;
	vector<Vec3f>point;
	vector<Vec3f>bzpoint;
};