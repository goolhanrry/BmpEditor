#include "BmpEditor.h"

int main()
{
	BmpEditor bmpEditor("assets\\source.bmp");

	if (bmpEditor.file)
	{
		bmpEditor.bmpReverse("assets\\source_reverse_output.bmp");
		bmpEditor.bmpOverlap("assets\\overlap.bmp", "assets\\source_overlap_output.bmp");
	}

	system("pause");

	return 0;
}