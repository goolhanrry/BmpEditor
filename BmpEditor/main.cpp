#include "BmpEditor.h"

int main()
{
	BmpEditor bmpEditor;

	bmpEditor.bmpReverse("assets\\source.bmp", "output\\reverse_output.bmp");
	bmpEditor.bmpOverlap("assets\\source.bmp", "assets\\overlap.bmp", "output\\overlap_output.bmp");
	bmpEditor.bmpFocus("assets\\source.bmp", "output\\focus_output.bmp");

	return 0;
}