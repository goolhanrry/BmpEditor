﻿#include "BmpEditor.h"

int main()
{
	BmpEditor bmpEditor;

	// 第二次实习
	bmpEditor.bmpReverse("assets\\source.bmp", "output\\1_reverse_output.bmp");
	bmpEditor.bmpOverlap("assets\\source.bmp", "assets\\overlap.bmp", "output\\2_overlap_output.bmp");
	bmpEditor.bmpFocus("assets\\source.bmp", "output\\3_focus_output.bmp", bmpEditor.averageTmp);

	// 第三次实习
	bmpEditor.bmp256to32b("assets\\source.bmp", "output\\4_256to32b_output.bmp");
	bmpEditor.distanceTransform("assets\\source.bmp", "output\\5_loc_output.bmp", "output\\5_dis_output.bmp", bmpEditor.octagonTmp, 0xff);

	// 第四次实习
	bmpEditor.generateBoundary("assets\\source.bmp", "output\\6_generate_boundary_output.bmp");
	bmpEditor.generateBuffer("output\\5_loc_output.bmp", "output\\5_dis_output.bmp", "output\\7_generate_buffer_output.bmp", 18);
	bmpEditor.adhesionTransform("assets\\source.bmp", "output\\8_adhesion_output.bmp", bmpEditor.manHattanTmp, 80, 20);
	bmpEditor.generateAxis("assets\\source.bmp", "output\\9_axics_output.bmp", bmpEditor.manHattanTmp);

	return 0;
}