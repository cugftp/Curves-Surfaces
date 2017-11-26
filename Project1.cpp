// Project1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
using namespace std;
#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"
// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) 
{
	//argc = 11;
	//char temp[11][30] = { "curve_editor","-input","spline09_teapot.txt" ,"-patch_tessellation",
	//	"30" ,"-curve_tessellation","30","-revolution_tessellation","100","-output","teapot_high.obj"};
	/////*char temp[9][30] = { "curve_editor","-input","output07_edit.txt" ,"-curve_tessellation",
	////	"10","-revolution_tessellation","60" ,"-output","vase_high.obj"};*/
	//for (int i = 0; i < argc; i++) 
	//{
	//	argv[i] = new char[20];
	//	argv[i] = temp[i];
	//}
	//argc = 4;
	/*char temp[4][30] = { "curve_editor","-input","spline08_bezier_patch.txt" ,"-gui"
		};
	for (int i = 0; i < argc; i++)
	{
		argv[i] = new char[20];
		argv[i] = temp[i];
	}*/
	// parse the command line arguments & input file
	ArgParser *args = new ArgParser(argc, argv);
	SplineParser* splines = new SplineParser(args->input_file);
	// launch curve editor!
	if (args->gui) 
	{
		GLCanvas glcanvas;
		glcanvas.initialize(args, splines);
		// this never returns...
	}
	// output as required
	splines->SaveBezier(args);
	splines->SaveBSpline(args);
	splines->SaveTriangles(args);
	// cleanup
	delete args;
	delete splines;
	return 0;
}

// ====================================================================
// ====================================================================




