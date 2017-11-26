#include "stdafx.h"
#include "surface.h"
#include<GL/glut.h>
SurfaceOfRevolution::SurfaceOfRevolution(Curve *c)
{
	C = c;
	points = c->getNumVertices();
	for (int i = 0; i <points; i++)
		point.push_back(c->getVertex(i));
	ch1 = c->ch;
}
void SurfaceOfRevolution::Paint(ArgParser * args)
{
	int yy = args->curve_tessellation;
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < points; i++)
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3f(point[i].x(), point[i].y(), point[i].z());
		glEnd();
	}
	glFlush();
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < points - 1; i++)
	{
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(point[i].x(), point[i].y(), point[i].z());
		glVertex3f(point[i+1].x(), point[i + 1].y(), point[i + 1].z());
		glEnd();
	}
	glFlush();
	if (ch1)
	{
		int counts = (points - 1) / 3;
		Vec3f v3 = Vec3f(point[0].x(), point[0].y(), point[0].z());
		for (int k = 0; k < counts; k++)
		{
			vector<Vec3f>v;
			v.push_back(v3);
			int q = k * 4;
			if (k >= 1)
				q = q - k;
			float x1 = point[q].x(), x2 = point[q + 1].x(),
				x3 = point[q + 2].x(), x4 = point[q + 3].x();
			float y1 = point[q].y(), y2 = point[q + 1].y(),
				y3 = point[q + 2].y(), y4 = point[q + 3].y();
			float z1 = point[q].z(), z2 = point[q + 1].z(),
				z3 = point[q + 2].z(), z4 = point[q + 3].z();
			for (int j = 1; j < yy; j++)
			{
				float i = float(j) / (yy*1.0);//主要是这里
				float a, b, c;
				a = (1 - i)*(1 - i)*(1 - i)*x1 + 3 * i*(1 - i)*(1 - i)*x2 + 3 * i*i*(1 - i)*x3 + i*i*i*x4;
				b = (1 - i)*(1 - i)*(1 - i)*y1 + 3 * i*(1 - i)*(1 - i)*y2 + 3 * i*i*(1 - i)*y3 + i*i*i*y4;
				c = (1 - i)*(1 - i)*(1 - i)*z1 + 3 * i*(1 - i)*(1 - i)*z2 + 3 * i*i*(1 - i)*z3 + i*i*i*z4;
				v.push_back(Vec3f(a, b, c));
			}
			v3 = Vec3f(x4, y4, z4);
			v.push_back(v3);
			glColor3f(0.0, 1.0, 0.0);
			for (int i = 0; i < v.size() - 1; i++)
			{
				glLineWidth(3);
				glBegin(GL_LINES);
				glVertex3f(v[i].x(), v[i].y(), v[i].z());
				glVertex3f(v[i + 1].x(), v[i + 1].y(), v[i + 1].z());
				glEnd();
			}
			glFlush();
		}
	}
	else
	{
		int counts = points - 3;
		vector<Vec3f>v;
		int q = 0;
		for (int k = 0; k < counts; k++)
		{
			float x1 = Spline::getVertex(q).x(), x2 = Spline::getVertex(q + 1).x(),
				x3 = Spline::getVertex(q + 2).x(), x4 = Spline::getVertex(q + 3).x();
			float y1 = Spline::getVertex(q).y(), y2 = Spline::getVertex(q + 1).y(),
				y3 = Spline::getVertex(q + 2).y(), y4 = Spline::getVertex(q + 3).y();
			float z1 = Spline::getVertex(q).z(), z2 = Spline::getVertex(q + 1).z(),
				z3 = Spline::getVertex(q + 2).z(), z4 = Spline::getVertex(q + 3).z();
			for (int j = 0; j <= yy; j++)
			{
				float i = float(j) / (yy*1.0);//主要是这里
				float a, b, c;
				a = ((1 - i)*(1 - i)*(1 - i)*x1 + (3 * i*i*i - 6 * i*i + 4)*x2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*x3 + i*i*i*x4) / 6.0;
				b = ((1 - i)*(1 - i)*(1 - i)*y1 + (3 * i*i*i - 6 * i*i + 4)*y2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*y3 + i*i*i*y4) / 6.0;
				c = ((1 - i)*(1 - i)*(1 - i)*z1 + (3 * i*i*i - 6 * i*i + 4)*z2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*z3 + i*i*i*z4) / 6.0;
				v.push_back(Vec3f(a, b, c));
			}
			q++;
		}
		glColor3f(0.0, 1.0, 0.0);
		for (int i = 0; i<(yy + 1)*counts - 1; i++)
		{
			glLineWidth(3);
			glBegin(GL_LINES);
			glVertex3f(v[i].x(), v[i].y(), v[i].z());
			glVertex3f(v[i + 1].x(), v[i + 1].y(), v[i + 1].z());
			glEnd();
		}
		glFlush();
	}

}
TriangleMesh * SurfaceOfRevolution::OutputTriangles(ArgParser * args)
{
	return C->OutputTriangles(args);
}
int SurfaceOfRevolution::getNumVertices()
{
	return points;
}
Vec3f SurfaceOfRevolution::getVertex(int i)
{
	return point[i];
}
void SurfaceOfRevolution::moveControlPoint(int selectedPoint, float x, float y)
{
	point[selectedPoint] = Vec3f(x, y, 0);
}
void SurfaceOfRevolution::addControlPoint(int selectedPoint, float x, float y)
{
	point.insert(point.begin() + selectedPoint, Vec3f(x, y, 0));
	points++;
}
void SurfaceOfRevolution::deleteControlPoint(int selectedPoint)
{
	if ((points >4))
	{
		point.erase(point.begin() + selectedPoint);
		points--;
	}
}
BezierPatch::BezierPatch()
{
	points=0;	
}
void BezierPatch::Paint(ArgParser * args)
{
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < points; i++)
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3f(point[i].x(), point[i].y(), point[i].z());
		glEnd();
	}
	glFlush();
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < points - 1; i++)
	{
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(point[i].x(), point[i].y(), point[i].z());
		glVertex3f(point[i + 1].x(), point[i + 1].y(), point[i + 1].z());
		glEnd();
	}
	glFlush();

}

void BezierPatch::set(int i, Vec3f v)
{
	point.push_back(v);
	points++;
}

int BezierPatch::getNumVertices()
{
	return points;
}

Vec3f BezierPatch::getVertex(int i)
{
	return point[i];
}

TriangleMesh * BezierPatch::OutputTriangles(ArgParser * args)
{
	SetCurveVertex(args);
	TriangleNet *net = new TriangleNet(args->patch_tessellation, args->patch_tessellation);
	for (int i = 0; i < args->patch_tessellation+1; i++)
	{		
		for (int j = 0; j < args->patch_tessellation+1; j++)
		{			
			net->SetVertex(i,j,bzpoint[i*(args->patch_tessellation+1)+j] );
		}
	}
	return net;
}

void BezierPatch::SetCurveVertex(ArgParser * args)
{
 	    vector<vector<Vec3f>>V;
		for (int k = 0; k < 4; k++)
		{
			int q = k*4;
			float x1 = point[q].x(), x2 = point[q + 1].x(),
				x3 = point[q + 2].x(), x4 = point[q + 3].x();
			float y1 = point[q].y(), y2 = point[q + 1].y(),
				y3 = point[q + 2].y(), y4 = point[q + 3].y();
			float z1 = point[q].z(), z2 = point[q + 1].z(),
				z3 = point[q + 2].z(), z4 = point[q + 3].z();
			vector<Vec3f>v;
			v.push_back(Vec3f(x1, y1, z1));
			for (int j = 1; j<args->patch_tessellation; j++)
			{
				float i = float(j)/(args->patch_tessellation*1.0);//主要是这里
				float a, b, c;
				a = (1 - i)*(1 - i)*(1 - i)*x1 + 3 * i*(1 - i)*(1 - i)*x2 + 3 * i*i*(1 - i)*x3 + i*i*i*x4;
				b = (1 - i)*(1 - i)*(1 - i)*y1 + 3 * i*(1 - i)*(1 - i)*y2 + 3 * i*i*(1 - i)*y3 + i*i*i*y4;
				c = (1 - i)*(1 - i)*(1 - i)*z1 + 3 * i*(1 - i)*(1 - i)*z2 + 3 * i*i*(1 - i)*z3 + i*i*i*z4;
				v.push_back(Vec3f(a, b, c));
			}			
			v.push_back(Vec3f(x4, y4, z4));
			V.push_back(v);
	   }
		for (int i = 0; i < args->patch_tessellation + 1; i++)
		{
			float x1 = V[0][i].x(), x2 = V[1][i].x(), x3 = V[2][i].x(), x4 = V[3][i].x();
			float y1 = V[0][i].y(), y2 = V[1][i].y(), y3 = V[2][i].y(), y4 = V[3][i].y();
			float z1 = V[0][i].z(), z2 = V[1][i].z(), z3 = V[2][i].z(), z4 = V[3][i].z();			
			bzpoint.push_back(Vec3f(x1, y1, z1));
			for (int j = 1; j<args->patch_tessellation; j++)
			{
				float i = float(j) / (args->patch_tessellation*1.0);//主要是这里
				float a, b, c;
				a = (1 - i)*(1 - i)*(1 - i)*x1 + 3 * i*(1 - i)*(1 - i)*x2 + 3 * i*i*(1 - i)*x3 + i*i*i*x4;
				b = (1 - i)*(1 - i)*(1 - i)*y1 + 3 * i*(1 - i)*(1 - i)*y2 + 3 * i*i*(1 - i)*y3 + i*i*i*y4;
				c = (1 - i)*(1 - i)*(1 - i)*z1 + 3 * i*(1 - i)*(1 - i)*z2 + 3 * i*i*(1 - i)*z3 + i*i*i*z4;
				bzpoint.push_back(Vec3f(a, b, c));
			}
			bzpoint.push_back(Vec3f(x4, y4, z4));			
		}
}
