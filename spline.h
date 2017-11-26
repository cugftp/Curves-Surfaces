#pragma once
#include"arg_parser.h"
#include"triangle_mesh.h"
#include<vector>

class Spline
{

public:
	Spline();
	// 用于画图的FOR VISUALIZATION
	virtual void Spline::Paint(ArgParser *args);

	// 用于实现样条类型转换的FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void Spline::OutputBezier(FILE *file);
	virtual void Spline::OutputBSpline(FILE *file);

	// 用于得到控制点的FOR CONTROL POINT PICKING
	virtual int Spline::getNumVertices();
	virtual Vec3f Spline::getVertex(int i);

	// 用于编辑操作的FOR EDITING OPERATIONS
	virtual void Spline::moveControlPoint(int selectedPoint, float x, float y);
	virtual void Spline::addControlPoint(int selectedPoint, float x, float y);
	virtual void Spline::deleteControlPoint(int selectedPoint);

	// 用于产生三角形的FOR GENERATING TRIANGLES
	virtual TriangleMesh* Spline::OutputTriangles(ArgParser *args);
	void set(int i, Vec3f v);
protected:
	vector<Vec3f>point;//存放点
	int points=0;//存放点数
};
