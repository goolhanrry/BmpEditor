#include "BmpEditor.h"

int main()
{
	BmpEditor bmpEditor;

	// 第二次实习
	bmpEditor.bmpReverse("assets\\source.bmp", "output\\reverse_output.bmp");
	bmpEditor.bmpOverlap("assets\\source.bmp", "assets\\overlap.bmp", "output\\overlap_output.bmp");
	bmpEditor.bmpFocus("assets\\source.bmp", "output\\focus_output.bmp");

	//第三次实习
	bmpEditor.bmp256to32b("assets\\source.bmp", "output\\256to32b_output.bmp");

	return 0;
}