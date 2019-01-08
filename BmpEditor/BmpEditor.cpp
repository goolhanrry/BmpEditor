#include <iostream>
#include "BmpEditor.h"
#include "AverageTmp.h"
using namespace std;

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

void BmpEditor::bmpFocus(const char * srcBmpName, const char* destBmpName)
{
	// 使用均值滤波
	AverageTmp *tmp = new AverageTmp;

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