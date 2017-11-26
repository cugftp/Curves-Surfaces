#include "stdafx.h"
#include "spline.h"
float const PI = 3.1415926;
Spline::Spline()
{

}

void Spline::Paint(ArgParser * args)
{
}

void Spline::OutputBezier(FILE * file)
{

}

void Spline::OutputBSpline(FILE * file)
{
	fprintf(file, "%c", '\n');
	fprintf(file, "%s%c", "surface_of_revolution", '\n');
	fprintf(file, "%s%c", "bspline", '\n');
	fprintf(file, "%s%d%c", "num_vertices ",Spline::getNumVertices(), '\n');
	for (int i = 0; i < points; i++)
	{
		fprintf(file, "%f%s%f%s%f%c", point[i].x()," ", point[i].y(), " ", point[i].z(),'\n');		
	}
}

int Spline::getNumVertices()
{
	return points;
}

Vec3f Spline::getVertex(int i)
{
	return point[i];
}

void Spline::moveControlPoint(int selectedPoint, float x, float y)
{
	point[selectedPoint] = Vec3f(x, y, 0);
}

void Spline::addControlPoint(int selectedPoint, float x, float y)
{		
		point.insert(point.begin() + selectedPoint, Vec3f(x, y, 0));
		points++;
}
void Spline::deleteControlPoint(int selectedPoint)
{
	if ((points >4))
	{
	point.erase(point.begin() + selectedPoint);
	points--;
	}
}

TriangleMesh * Spline::OutputTriangles(ArgParser * args)
{
	int counts = points-3;	
	vector<Vec3f>V;
	int q = 0;
	for (int k = 0; k < counts; k++)
	{
		float x1 =point[q].x(), x2 = point[q+1].x(),
			x3 = point[q + 2].x(), x4 = point[q + 3].x();
		float y1 =point[q].y(), y2 = point[q + 1].y(),
			y3 =point[q + 2].y(), y4 = point[q + 3].y();
		float z1 = point[q].z(), z2 = point[q + 1].z(),
			z3 =point[q + 2].z(), z4 =point[q + 3].z();
		for (int j = 0; j <=args->curve_tessellation; j++)
		{
			float i = float(j) / (args->curve_tessellation*1.0);//主要是这里
			float a, b, c;
			a = ((1 - i)*(1 - i)*(1 - i)*x1 + (3 * i*i*i - 6 * i*i + 4)*x2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*x3 + i*i*i*x4) / 6.0;
			b = ((1 - i)*(1 - i)*(1 - i)*y1 + (3 * i*i*i - 6 * i*i + 4)*y2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*y3 + i*i*i*y4) / 6.0;
			c = ((1 - i)*(1 - i)*(1 - i)*z1 + (3 * i*i*i - 6 * i*i + 4)*z2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*z3 + i*i*i*z4) / 6.0;
			V.push_back(Vec3f(a, b, c));
		}
		q++;
	}
	int vnum = args->curve_tessellation*(points-3) +points/3;
	TriangleNet *net = new TriangleNet(args->revolution_tessellation, vnum - 1);
	vector<Vec3f>vv;
	for (int i = 0; i < V.size(); i++)
	{
		vv.push_back(V[i]);
	}
	V.clear();
	for (int i = 0; i < args->revolution_tessellation + 1; i++)
	{
		float angle = (2 * i*PI) / args->revolution_tessellation;
		for (int j = 0; j < vnum; j++)
		{
			float aa = (vv[j].x())*cos(angle) + (vv[j].z())*sin(angle);
			float bb = vv[j].y();
			float cc = (vv[j].z())*cos(angle) - (vv[j].x())*sin(angle);
			net->SetVertex(i, j, Vec3f(aa, bb, cc));
		}
	}
	return net;
}

void Spline::set(int i, Vec3f v)
{
	point.push_back(v);
	points++;
}
