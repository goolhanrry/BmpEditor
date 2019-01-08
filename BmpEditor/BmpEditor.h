#include <iostream>
#include <windows.h>
#include <wingdi.h>

class BmpEditor
{
public:
	BmpEditor(const char *srcBmpName);
	~BmpEditor();
	void bmpReverse(const char *destBmpName);
	void bmpOverlap(const char *newBmpName, const char *destBmpName);

	FILE *file;

private:
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char clrTab[256 * 4];
	unsigned mtxWidth, mtxHeight;
	unsigned char **mtxData;
};