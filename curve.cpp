#include "stdafx.h"
#include "curve.h"
#include"matrix.h"
#include<GL/glut.h> 
float const PI = 3.1415926;
BezierCurve::BezierCurve(int i)
{
	bzpoints = i;
	ch = true;
}
Curve::Curve()
{

}
void BezierCurve::Paint(ArgParser *args)
{	
	int yy = args->curve_tessellation;
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i <bzpoints ; i++)
	{
		glPointSize(5);
		glBegin(GL_POINTS);		
		glVertex3f(Spline::getVertex(i).x(), Spline::getVertex(i).y(), Spline::getVertex(i).z());
		glEnd();
	}
	glFlush();
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < bzpoints-1; i++)
	{
		glLineWidth(1);
		glBegin(GL_LINES);		
		glVertex3f(Spline::getVertex(i).x(), Spline::getVertex(i).y(), Spline::getVertex(i).z());
		glVertex3f(Spline::getVertex(i+1).x(), Spline::getVertex(i+1).y(), Spline::getVertex(i+1).z());
		glEnd();
	}
	glFlush();	
	int counts = (bzpoints-1) / 3;
	Vec3f v3 = Vec3f(Spline::getVertex(0).x(), Spline::getVertex(0).y(), Spline::getVertex(0).z());
	for (int k = 0; k < counts; k++)
	{
		vector<Vec3f>v;
		v.push_back(v3);
		int q = k * 4;
		if (k >= 1)
			q = q - k;
		float x1 = Spline::getVertex(q).x(), x2 = Spline::getVertex(q+1).x(),
			x3 = Spline::getVertex(q+2).x(), x4 = Spline::getVertex(q + 3).x();
		float y1 = Spline::getVertex(q + 0).y(), y2 = Spline::getVertex(q + 1).y(),
			y3 = Spline::getVertex(q + 2).y(), y4 = Spline::getVertex(q + 3).y();
		float z1 = Spline::getVertex(q+ 0).z(), z2 = Spline::getVertex(q + 1).z(),
			z3 = Spline::getVertex(q+ 2).z(), z4 = Spline::getVertex(q + 3).z();		
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
void BezierCurve::OutputBezier(FILE * file)
{
	fprintf(file, "%c", '\n');
	fprintf(file,"%s%c", "bezier",'\n');
	fprintf(file, "%s%d%c", "num_vertices ",4,'\n');
	for (int i = 0; i < Spline::getNumVertices(); i++)
	{
		fprintf(file, "%d",(int) Spline::getVertex(i).x());
		fprintf(file, "%s", " ");
		fprintf(file, "%d", (int)Spline::getVertex(i).y());
		fprintf(file, "%s", " ");
		fprintf(file, "%d%c", (int)Spline::getVertex(i).z(),'\n');
	}
	fprintf(file, "%c",'\n');
}
void BezierCurve::OutputBSpline(FILE * file)
{
	float Bb[16] = { -1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0};
	float Bs[16] = { -1,3,-3,1 ,3,-6,0,4,-3,3, 3,1,1,0,0,0};
	float Gb[16] = { 
		             Spline::getVertex(0).x(),Spline::getVertex(1).x(),Spline::getVertex(2).x(),Spline::getVertex(3).x(),
		             Spline::getVertex(0).y(),Spline::getVertex(1).y(),Spline::getVertex(2).y(),Spline::getVertex(3).y(),
		             Spline::getVertex(0).z(),Spline::getVertex(1).z(),Spline::getVertex(2).z(),Spline::getVertex(3).z(),
		             0,0,0,0 };
	Matrix BB(Bb);
	Matrix BS(Bs);
	Matrix GB(Gb);
	Matrix GS;
	BS.Inverse();
	GS =6* GB*BB*BS;
	float* GS_matrix;
	GS_matrix = GS.glGet();
	fprintf(file, "%c", '\n');
	fprintf(file, "%s%c", "bspline", '\n');
	fprintf(file, "%s%d%c", "num_vertices ", 4, '\n');
	for (int i = 0; i < 16; i++)
	{
		if ((i+1) % 4== 0)
		{
			continue;
		}
		else
		{
			fprintf(file, "%f", GS_matrix[i]);
			fprintf(file, "%s", " ");
			i = i + 1;
			fprintf(file, "%f", GS_matrix[i]);
			fprintf(file, "%s", " ");
			i = i + 1;
			fprintf(file, "%f%c", GS_matrix[i], '\n');					
		}
	}
	fprintf(file, "%c", '\n');
}
TriangleMesh * BezierCurve::OutputTriangles(ArgParser * args)
{
	int counts = (bzpoints - 1) / 3;
	Vec3f v3 = Vec3f(Spline::getVertex(0).x(), Spline::getVertex(0).y(), Spline::getVertex(0).z());
	vector<Vec3f>V;
	for (int k = 0; k < counts; k++)
	{
		V.push_back(v3);
		int q = k * 4;
		if (k >= 1)
			q = q - k;
		float x1 = Spline::getVertex(q).x(), x2 = Spline::getVertex(q + 1).x(),
			x3 = Spline::getVertex(q + 2).x(), x4 = Spline::getVertex(q + 3).x();
		float y1 = Spline::getVertex(q + 0).y(), y2 = Spline::getVertex(q + 1).y(),
			y3 = Spline::getVertex(q + 2).y(), y4 = Spline::getVertex(q + 3).y();
		float z1 = Spline::getVertex(q + 0).z(), z2 = Spline::getVertex(q + 1).z(),
			z3 = Spline::getVertex(q + 2).z(), z4 = Spline::getVertex(q + 3).z();
		for (int j = 1; j < args->curve_tessellation; j++)
		{
			float i = float(j) / (args->curve_tessellation*1.0);//主要是这里
			float a, b, c;
			a = (1 - i)*(1 - i)*(1 - i)*x1 + 3 * i*(1 - i)*(1 - i)*x2 + 3 * i*i*(1 - i)*x3 + i*i*i*x4;
			b = (1 - i)*(1 - i)*(1 - i)*y1 + 3 * i*(1 - i)*(1 - i)*y2 + 3 * i*i*(1 - i)*y3 + i*i*i*y4;
			c = (1 - i)*(1 - i)*(1 - i)*z1 + 3 * i*(1 - i)*(1 - i)*z2 + 3 * i*i*(1 - i)*z3 + i*i*i*z4;
			V.push_back(Vec3f(a, b, c));
		}
		v3 = Vec3f(x4, y4, z4);
		V.push_back(v3);
	}
	int vnum = args->curve_tessellation*(bzpoints / 3) + bzpoints / 3;
	TriangleNet *net = new TriangleNet(args->revolution_tessellation,vnum-1);
	vector<Vec3f>vv;
	for (int i = 0; i < V.size(); i++)
	{
		vv.push_back(V[i]);
	}
	V.clear();
	for (int i = 0; i < args->revolution_tessellation + 1; i++)
	{
		float angle = (2*i*PI) / args->revolution_tessellation;
		for (int j = 0; j < vnum; j++)
		{				
			float aa = (vv[j].x())*cos(angle)+(vv[j].z())*sin(angle);
			float bb = vv[j].y();
			float cc = (vv[j].z())*cos(angle) - (vv[j].x())*sin(angle);			
			net->SetVertex(i, j, Vec3f(aa, bb, cc));
		}
	}
	return net;
}
BSplineCurve::BSplineCurve(int i)
{
	ch = false;
	bspoints =i;
}

void BSplineCurve::Paint(ArgParser * args)
{
	int yy = args->curve_tessellation;
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i <bspoints; i++)
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3f(Spline::getVertex(i).x(), Spline::getVertex(i).y(), Spline::getVertex(i).z());
		glEnd();
	}
	glFlush();
	glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < bspoints- 1; i++)
	{
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(Spline::getVertex(i).x(), Spline::getVertex(i).y(), Spline::getVertex(i).z());
		glVertex3f(Spline::getVertex(i + 1).x(), Spline::getVertex(i + 1).y(), Spline::getVertex(i + 1).z());
		glEnd();
	}
	glFlush();
	int counts = bspoints-3;	
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
			float i = float(j) /(yy*1.0);//主要是这里
			float a, b, c;
			a = ((1 - i)*(1 - i)*(1 - i)*x1 + (3 * i*i*i - 6 * i*i + 4)*x2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*x3 + i*i*i*x4) / 6.0;
			b = ((1 - i)*(1 - i)*(1 - i)*y1 + (3 * i*i*i - 6 * i*i + 4)*y2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*y3 + i*i*i*y4) / 6.0;
			c = ((1 - i)*(1 - i)*(1 - i)*z1 + (3 * i*i*i - 6 * i*i + 4)*z2 + ((-3)*i*i*i + 3 * i*i + 3 * i + 1)*z3 + i*i*i*z4) / 6.0;
			v.push_back(Vec3f(a, b, c));			
		}			
		q++;
	}
	glColor3f(0.0, 1.0, 0.0);
	for (int i=0; i<(yy+1)*counts-1; i++)
	{
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex3f(v[i].x(), v[i].y(), v[i].z());
		glVertex3f(v[i + 1].x(), v[i + 1].y(), v[i + 1].z());
		glEnd();
	}
	glFlush();
}
void BSplineCurve::OutputBezier(FILE * file)
{
	float Bb[16] = { -1,3,-3,1,3,-6,3,0,-3,3,0,0,1,0,0,0 };
	float Bs[16] = { -1,3,-3,1 ,3,-6,0,4,-3,3, 3,1,1,0,0,0 };
	float Gs[16] = {
		Spline::getVertex(0).x(),Spline::getVertex(1).x(),Spline::getVertex(2).x(),Spline::getVertex(3).x(),
		Spline::getVertex(0).y(),Spline::getVertex(1).y(),Spline::getVertex(2).y(),Spline::getVertex(3).y(),
		Spline::getVertex(0).z(),Spline::getVertex(1).z(),Spline::getVertex(2).z(),Spline::getVertex(3).z(),
		0,0,0,0 };
	Matrix BB(Bb);
	Matrix BS(Bs);
	Matrix GS(Gs);
	Matrix GB;
	BB.Inverse();
	GB = (1/float(6.0))*GS*BS*BB;
	float* GB_matrix;
	GB_matrix = GB.glGet();
	fprintf(file, "%c", '\n');
	fprintf(file, "%s%c", "bezier", '\n');
	fprintf(file, "%s%d%c", "num_vertices ", 4, '\n');
	for (int i = 0; i < 16; i++)
	{
		if ((i + 1) % 4 == 0)
		{
			continue;
		}
		else
		{
			fprintf(file, "%f", GB_matrix[i]);
			fprintf(file, "%s", " ");
			i = i + 1;
			fprintf(file, "%f", GB_matrix[i]);
			fprintf(file, "%s", " ");
			i = i + 1;
			fprintf(file, "%f%c", GB_matrix[i], '\n');
		}
	}
	fprintf(file, "%c", '\n');
}

void BSplineCurve::OutputBSpline(FILE * file)
{
	fprintf(file, "%c", '\n');
	fprintf(file, "%s%c", "bspline", '\n');
	fprintf(file, "%s%d%c", "num_vertices ", 4, '\n');
	for (int i = 0; i < Spline::getNumVertices(); i++)
	{
		fprintf(file, "%d", (int)Spline::getVertex(i).x());
		fprintf(file, "%s", " ");
		fprintf(file, "%d", (int)Spline::getVertex(i).y());
		fprintf(file, "%s", " ");
		fprintf(file, "%d%c", (int)Spline::getVertex(i).z(), '\n');
	}
	fprintf(file, "%c", '\n');
}
