#include <iostream>
#include <windows.h>
#include <wingdi.h>
#include "AverageTmp.h"
#include "OctagonTmp.h"
#include "ManHattanTmp.h"

class BmpEditor
{
public:
	BmpEditor();
	~BmpEditor();
	void bmpReverse(const char *srcBmpName, const char *destBmpName);
	void bmpOverlap(const char *srcBmpName, const char *newBmpName, const char *destBmpName);
	void bmpFocus(const char *srcBmpName, const char *destBmpName, Template *tmp);
	void bmp256to32b(const char *srcBmpName, const char *destBmpName);
	void distanceTransform(const char *srcBmpName, const char *locBmpName, const char *disBmpName, Template *tmp, unsigned char maxColor);
	void generateBoundary(const char *srcBmpName, const char *destBmpName);
	void generateBuffer(const char *locBmpName, const char *disBmpName, const char *destBmpName, float radius);
	void adhesionTransform(const char *srcBmpName, const char *destBmpName, Template *tmp, float outRadius, float inRadius);
	void generateAxis(const char *srcBmpName, const char *destBmpName, Template *tmp);

	AverageTmp *averageTmp;
	OctagonTmp *octagonTmp;
	ManHattanTmp *manHattanTmp;

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