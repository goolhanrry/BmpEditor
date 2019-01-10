#include <iostream>
#include "BmpEditor.h"
using namespace std;

BmpEditor::BmpEditor()
{
	averageTmp = new AverageTmp();
	octagonTmp = new OctagonTmp();
}

BmpEditor::~BmpEditor()
{
	delete averageTmp;
	delete octagonTmp;

	averageTmp = nullptr;
	octagonTmp = nullptr;
}

void BmpEditor::releaseRAM(unsigned char **mtxData, unsigned mtxHeight)
{
	if (mtxData)
	{
		// 释放无用内存
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			delete[] mtxData[i];
			mtxData[i] = nullptr;
		}

		delete[] mtxData;
		mtxData = nullptr;
	}
}

void BmpEditor::loadBmp(const char *srcBmpName)
{
	// 打开源文件
	if (fopen_s(&file, srcBmpName, "rb"))
	{
		cout << "Source file open error!" << endl;
		return;
	}

	// 读取位图基本信息
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fread(clrTab, 1024, 1, file);

	// 读取位图矩阵大小
	mtxWidth = (bmpInfoHeader.biWidth + 3) / 4 * 4;
	mtxHeight = bmpInfoHeader.biHeight;

	// 读取位图矩阵数据
	mtxData = new unsigned char*[mtxHeight];
	for (unsigned i = 0; i < mtxHeight; i++)
	{
		mtxData[i] = new unsigned char[mtxWidth];
		for (unsigned j = 0; j < mtxWidth; j++)
		{
			fread(&mtxData[i][j], sizeof(unsigned char), 1, file);
		}
	}

	// 关闭文件
	fclose(file);
}

void BmpEditor::saveBmp(const char *destBmpName, BITMAPFILEHEADER bmpFileHeader, BITMAPINFOHEADER bmpInfoHeader, unsigned char clrTab[], unsigned mtxWidth, unsigned mtxHeight, unsigned char **mtxBuf)
{
	// 打开结果位图文件
	if (fopen_s(&file, destBmpName, "wb"))
	{
		cout << "Output file open error!" << endl;
		return;
	}

	// 写入位图基本信息
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(clrTab, 1024, 1, file);

	// 写入转换结果
	for (unsigned i = 0; i < mtxHeight; i++)
	{
		fwrite(mtxBuf[i], 1, mtxWidth, file);
	}

	// 关闭文件
	fclose(file);
}

void BmpEditor::bmpReverse(const char *srcBmpName, const char *destBmpName)
{
	// 打开源文件
	loadBmp(srcBmpName);

	try
	{
		// 初始化输出矩阵缓存
		unsigned char **mtxBuf = new unsigned char*[mtxHeight];

		// 反色处理
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			mtxBuf[i] = new unsigned char[mtxWidth];
			for (unsigned j = 0; j < mtxWidth; j++)
			{
				mtxBuf[i][j] = mtxData[i][j] == 0xFF ? 0xFF : 255 - mtxData[i][j];
			}
		}

		// 保存结果文件
		saveBmp(destBmpName, bmpFileHeader, bmpInfoHeader, clrTab, mtxWidth, mtxHeight, mtxBuf);

		// 释放无用内存
		releaseRAM(mtxData, mtxHeight);
		releaseRAM(mtxBuf, mtxHeight);

		cout << "Reverse success" << endl;
	}
	catch (...)
	{
		cout << "Reverse failed" << endl;
	}
}

void BmpEditor::bmpOverlap(const char *srcBmpName, const char *newBmpName, const char *destBmpName)
{
	// 打开源文件
	loadBmp(srcBmpName);

	try
	{
		BITMAPFILEHEADER newBmpFileHeader;
		BITMAPINFOHEADER newBmpInfoHeader, outputBmpInfoHeader;
		unsigned char newClrTab[256 * 4];
		unsigned newMtxWidth, newMtxHeight, outputMtxWidth, outputMtxHeight;
		unsigned char **newMtxData, **mtxBuf;

		// 打开新文件
		if (fopen_s(&file, newBmpName, "rb"))
		{
			cout << "Overlap file not found!" << endl;
			return;
		}

		// 读取位图基本信息
		fread(&newBmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&newBmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		fread(newClrTab, 1024, 1, file);

		// 读取位图矩阵大小
		newMtxWidth = (newBmpInfoHeader.biWidth + 3) / 4 * 4;
		newMtxHeight = newBmpInfoHeader.biHeight;

		// 读取位图矩阵数据
		newMtxData = new unsigned char*[newMtxHeight];
		for (unsigned i = 0; i < newMtxHeight; i++)
		{
			newMtxData[i] = new unsigned char[newMtxWidth];
			for (unsigned j = 0; j < newMtxWidth; j++)
			{
				fread(&newMtxData[i][j], sizeof(unsigned char), 1, file);
			}
		}

		// 关闭文件
		fclose(file);

		// 计算输出矩阵大小
		outputBmpInfoHeader = bmpInfoHeader.biWidth < newBmpInfoHeader.biWidth ? bmpInfoHeader : newBmpInfoHeader;
		outputBmpInfoHeader.biHeight = bmpInfoHeader.biHeight < newBmpInfoHeader.biHeight ? bmpInfoHeader.biHeight : newBmpInfoHeader.biHeight;
		outputMtxWidth = (outputBmpInfoHeader.biWidth + 3) / 4 * 4;
		outputMtxHeight = outputBmpInfoHeader.biHeight;

		// 生成输出矩阵
		mtxBuf = new unsigned char*[outputMtxHeight];
		for (unsigned i = 0; i < outputMtxHeight; i++)
		{
			mtxBuf[i] = new unsigned char[outputMtxWidth];
			for (unsigned j = 0; j < outputMtxWidth; j++)
			{
				mtxBuf[i][j] = mtxData[i][j] + newMtxData[i][j] + 1;
			}
		}

		// 保存结果文件
		saveBmp(destBmpName, bmpFileHeader, bmpInfoHeader, clrTab, mtxWidth, mtxHeight, mtxBuf);

		// 释放无用内存
		releaseRAM(mtxData, mtxHeight);
		releaseRAM(mtxBuf, mtxHeight);
		releaseRAM(newMtxData, newMtxHeight);

		cout << "Overlap success" << endl;
	}
	catch (...)
	{
		cout << "Overlap failed" << endl;
	}
}

void BmpEditor::bmpFocus(const char * srcBmpName, const char* destBmpName, Template *tmp)
{
	// 打开源文件
	loadBmp(srcBmpName);

	try
	{
		// 初始化输出矩阵缓存
		unsigned char **mtxBuf = new unsigned char *[mtxHeight];
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			mtxBuf[i] = new unsigned char[mtxWidth];
			for (unsigned j = 0; j < mtxWidth; j++)
			{
				mtxBuf[i][j] = 0xff;
			}
		}

		// 平滑处理
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			for (unsigned j = 0; j < mtxWidth; j++)
			{
				if (mtxData[i][j] == 0xff)
				{
					continue;
				}

				unsigned char sum = 0;
				for (unsigned k = 0; k < tmp->getSize(); k++)
				{
					// 获取当前处理的坐标
					int currX = j + tmp->getOffsetX(k);
					int currY = i + tmp->getOffsetY(k);

					// 若超出范围则跳过
					if ((currX < 0) || (currX > mtxWidth - 1) || (currY < 0) || (currY > mtxHeight - 1))
					{
						continue;
					}

					// 加权求和
					sum += mtxData[currY][currX] * tmp->getWeight(k);
				}

				// 加权平均
				mtxBuf[i][j] = sum / tmp->getSize();
			}
		}

		// 保存结果文件
		saveBmp(destBmpName, bmpFileHeader, bmpInfoHeader, clrTab, mtxWidth, mtxHeight, mtxBuf);

		// 释放无用内存
		releaseRAM(mtxData, mtxHeight);
		releaseRAM(mtxBuf, mtxHeight);
		delete tmp;

		cout << "Focus success" << endl;
	}
	catch (...)
	{
		cout << "Focus failed" << endl;
	}
}

void BmpEditor::bmp256to32b(const char* srcBmpName, const char* destBmpName)
{
	try
	{
		// 打开源文件
		if (fopen_s(&file, srcBmpName, "rb"))
		{
			cout << "Source file open error!" << endl;
			return;
		}

		// 读取位图基本信息
		fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		fread(clrTab, 1024, 1, file);

		// 读取位图矩阵大小
		mtxWidth = (bmpInfoHeader.biWidth + 3) / 4 * 4;
		mtxHeight = bmpInfoHeader.biHeight;

		// 文件格式校验
		if (bmpInfoHeader.biBitCount != 8)
		{
			cout << "Format error!" << endl;
			return;
		}

		// 编制实体索引
		BITMAPFILEHEADER outputFileHeader;
		BITMAPINFOHEADER outputInfoHeader;
		int LineBytes = (bmpInfoHeader.biWidth + 3) / 4 * 4;
		int LineBytes2 = bmpInfoHeader.biWidth;

		outputFileHeader.bfType = bmpFileHeader.bfType;
		outputFileHeader.bfSize = 54 + LineBytes2 * 4 * bmpInfoHeader.biHeight;
		outputFileHeader.bfReserved1 = 0;
		outputFileHeader.bfReserved2 = 0;
		outputFileHeader.bfOffBits = 54;

		outputInfoHeader.biSize = 40;
		outputInfoHeader.biWidth = bmpInfoHeader.biWidth;
		outputInfoHeader.biHeight = bmpInfoHeader.biHeight;
		outputInfoHeader.biPlanes = 1;
		outputInfoHeader.biBitCount = 32;
		outputInfoHeader.biCompression = 0;
		outputInfoHeader.biSizeImage = 0;
		outputInfoHeader.biXPelsPerMeter = 0;
		outputInfoHeader.biYPelsPerMeter = 0;
		outputInfoHeader.biClrUsed = 0;
		outputInfoHeader.biClrImportant = 0;

		// 构造原始矩阵
		unsigned char *pSrcCellLineBuf = new unsigned char[LineBytes];          // 源文件行缓冲
		unsigned **pIdxCellMatixBuf = new unsigned*[outputInfoHeader.biHeight]; // 索引矩阵

		for (unsigned i = 0; i < outputInfoHeader.biHeight; i++)
		{
			fseek(file, bmpFileHeader.bfOffBits + i * sizeof(char)*LineBytes, 0);
			fread(pSrcCellLineBuf, sizeof(char), LineBytes, file);
			pIdxCellMatixBuf[i] = new unsigned[LineBytes2];
			for (unsigned j = 0; j < LineBytes2; j++)
			{
				if (pSrcCellLineBuf[j] == 0xFF)
				{
					pIdxCellMatixBuf[i][j] = 0xFFFFFFFF;
				}
				else
				{
					int k = pSrcCellLineBuf[j];
					unsigned uintTmp = k;

					// 取对应颜色
					uintTmp = clrTab[k * 4];
					uintTmp <<= 8;
					uintTmp += clrTab[k * 4 + 1];
					uintTmp <<= 8;
					uintTmp += clrTab[k * 4 + 2];
					uintTmp <<= 8;
					uintTmp += clrTab[k * 4 + 3];

					pIdxCellMatixBuf[i][j] = uintTmp;
				}
			}
		}

		// 打开结果位图文件
		FILE *outputFile;
		if (fopen_s(&outputFile, destBmpName, "wb"))
		{
			cout << "Output file open error!" << endl;
			return;
		}

		// 写入位图基本信息
		fwrite(&outputFileHeader, sizeof(BITMAPFILEHEADER), 1, outputFile);
		fwrite(&outputInfoHeader, sizeof(BITMAPINFOHEADER), 1, outputFile);
		fwrite(clrTab, 1024, 1, outputFile);

		// 写入转换结果
		for (unsigned i = 0; i < outputInfoHeader.biHeight; i++)
		{
			fseek(outputFile, 54 + i * sizeof(unsigned)*LineBytes2, 0);
			fwrite(pIdxCellMatixBuf[i], sizeof(unsigned), LineBytes2, outputFile);
			delete[] pIdxCellMatixBuf[i];
			pIdxCellMatixBuf[i] = nullptr;
		}

		delete[] pIdxCellMatixBuf;
		delete[] pSrcCellLineBuf;

		// 关闭文件
		fclose(file);
		fclose(outputFile);

		cout << "Transform success" << endl;
	}
	catch (...)
	{
		cout << "Transform failed" << endl;
	}
}

void BmpEditor::distanceTransform(const char *srcBmpName, Template *tmp, const char *locBmpName, const char *disBmpName)
{
	try
	{
		// 打开源文件
		if (fopen_s(&file, srcBmpName, "rb"))
		{
			cout << "Source file open error!" << endl;
			return;
		}

		// 读取位图基本信息
		fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		fread(clrTab, 1024, 1, file);

		// 读取位图矩阵大小
		mtxWidth = (bmpInfoHeader.biWidth + 3) / 4 * 4;
		mtxHeight = bmpInfoHeader.biHeight;

		// 建立距离场和分配场
		unsigned char **locMtx = new unsigned char *[mtxHeight];
		float  **disMtx = new float *[mtxHeight];

		for (unsigned i = 0; i < mtxHeight; i++)
		{
			locMtx[i] = new unsigned char[mtxWidth];
			disMtx[i] = new float[mtxWidth];
			fread(locMtx[i], sizeof(unsigned char), mtxWidth, file);
			for (unsigned j = 0; j < bmpInfoHeader.biWidth; j++)
			{
				disMtx[i][j] = locMtx[i][j] == 255 ? FLT_MAX : 0.0;
			}
		}

		// 关闭文件
		fclose(file);

		// 模板距离变换
		for (int y = 0; y < mtxHeight; y++)
		{
			for (int x = 0; x < bmpInfoHeader.biWidth; x++)
			{
				float tmpMin = disMtx[y][x] + 0;

				if (fabs(tmpMin) < FLT_MIN)
				{
					continue;
				}

				for (unsigned k = 0; k < tmp->getSize() / 2; k++)
				{
					int currX = x + tmp->getOffsetX(k);
					int currY = y + tmp->getOffsetY(k);
					float tempDis = tmp->getWeight(k);

					if ((currX < 0) || (currX > bmpInfoHeader.biWidth - 1) || (currY < 0) || (currY > mtxHeight - 1))
					{
						continue;
					}

					if (fabs(disMtx[currY][currX] - FLT_MAX) < FLT_MIN)
					{
						continue;
					}

					if (disMtx[currY][currX] + tempDis < tmpMin)
					{
						tmpMin = tempDis + disMtx[currY][currX];
						disMtx[y][x] = tmpMin;
						locMtx[y][x] = locMtx[currY][currX];
					}
				}
			}
		}

		for (int y = mtxHeight - 1; y >= 0; y--)
		{
			for (int x = bmpInfoHeader.biWidth - 1; x >= 0; x--)
			{
				float tmpMin = disMtx[y][x] + 0;

				if (fabs(tmpMin) < FLT_MIN)
				{
					continue;
				}

				for (unsigned k = tmp->getSize() - 1; k > tmp->getSize() / 2; k--)
				{
					int currX = x + tmp->getOffsetX(k);
					int currY = y + tmp->getOffsetY(k);
					float tempDis = tmp->getWeight(k);

					if ((currX < 0) || (currX > bmpInfoHeader.biWidth - 1) || (currY < 0) || (currY > mtxHeight - 1))
					{
						continue;
					}

					if (fabs(disMtx[currY][currX] - FLT_MAX) < FLT_MIN)
					{
						continue;
					}

					if (disMtx[currY][currX] + tempDis < tmpMin)
					{
						tmpMin = tempDis + disMtx[currY][currX];
						disMtx[y][x] = tmpMin;
						locMtx[y][x] = locMtx[currY][currX];
					}
				}
			}
		}

		// 写入结果文件
		BITMAPFILEHEADER outputFileHeader;
		BITMAPINFOHEADER outputInfoHeader;

		outputFileHeader.bfType = bmpFileHeader.bfType;
		outputFileHeader.bfSize = 54 + bmpInfoHeader.biWidth * 4 * bmpInfoHeader.biHeight;
		outputFileHeader.bfReserved1 = 0;
		outputFileHeader.bfReserved2 = 0;
		outputFileHeader.bfOffBits = 54;

		outputInfoHeader.biSize = 40;
		outputInfoHeader.biWidth = bmpInfoHeader.biWidth;
		outputInfoHeader.biHeight = bmpInfoHeader.biHeight;
		outputInfoHeader.biPlanes = 1;
		outputInfoHeader.biBitCount = 32;
		outputInfoHeader.biCompression = 0;
		outputInfoHeader.biSizeImage = 0;
		outputInfoHeader.biXPelsPerMeter = 0;
		outputInfoHeader.biYPelsPerMeter = 0;
		outputInfoHeader.biClrUsed = 0;
		outputInfoHeader.biClrImportant = 0;

		// 保存距离场
		fopen_s(&file, locBmpName, "wb");
		fwrite(&outputFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fwrite(&outputInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			fwrite(disMtx[i], sizeof(float), bmpInfoHeader.biWidth, file);
		}
		fclose(file);

		// 保存分配场
		fopen_s(&file, disBmpName, "wb");
		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		fwrite(clrTab, 1024, 1, file);
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			fwrite(locMtx[i], 1, mtxWidth, file);
		}
		fclose(file);

		// 释放无用内存
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			delete[] locMtx[i];
			delete[] disMtx[i];

			locMtx[i] = nullptr;
			disMtx[i] = nullptr;
		}

		delete[] locMtx;
		delete[] disMtx;

		locMtx = nullptr;
		disMtx = nullptr;

		cout << "Distance transform success" << endl;
	}
	catch (...)
	{
		cout << "Distance transform failed" << endl;
	}
}