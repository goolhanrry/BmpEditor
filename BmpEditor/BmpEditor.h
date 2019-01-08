#include <iostream>
#include <windows.h>
#include <wingdi.h>

class BmpEditor
{
public:
	void bmpReverse(const char *srcBmpName, const char *destBmpName);
	void bmpOverlap(const char *srcBmpName, const char *newBmpName, const char *destBmpName);
	void bmpFocus(const char *srcBmpName, const char *destBmpName);

private:
	void releaseRAM(unsigned char **mtxData, unsigned mtxHeight);
	void loadBmp(const char *srcBmpName);
	void saveBmp(const char *destBmpName, BITMAPFILEHEADER bmpFileHeader, BITMAPINFOHEADER bmpInfoHeader, unsigned char clrTab[], unsigned mtxWidth, unsigned mtxHeight, unsigned char **mtxBuf);

	FILE *file;
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char clrTab[256 * 4];
	unsigned mtxWidth, mtxHeight;
	unsigned char **mtxData;
};