#include <iostream>
#include "BmpEditor.h"
using namespace std;

BmpEditor::BmpEditor(const char *srcBmpName)
{
	// 打开源文件
	if (fopen_s(&file, srcBmpName, "rb"))
	{
		cout << "Source file not found!" << endl;
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

BmpEditor::~BmpEditor()
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

void BmpEditor::bmpReverse(const char *destBmpName)
{
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

		// 打开结果位图文件
		if (fopen_s(&file, destBmpName, "wb"))
		{
			cout << "Reverse output file open error!" << endl;
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

		// 释放无用内存
		for (unsigned i = 0; i < mtxHeight; i++)
		{
			delete[] mtxBuf[i];
			mtxBuf[i] = nullptr;
		}

		delete[] mtxBuf;
		mtxBuf = nullptr;

		cout << "Reverse success" << endl;
	}
	catch (...)
	{
		cout << "Reverse failed" << endl;
	}
}

void BmpEditor::bmpOverlap(const char *newBmpName, const char *destBmpName)
{
	try
	{
		BITMAPFILEHEADER newBmpFileHeader;
		BITMAPINFOHEADER newBmpInfoHeader, outputBmpInfoHeader;
		unsigned char newClrTab[256 * 4];
		unsigned newMtxWidth, newMtxHeight, outputMtxWidth, outputMtxHeight;
		unsigned char **newMtxData, **outputMtxData;

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
		outputMtxData = new unsigned char*[outputMtxHeight];
		for (unsigned i = 0; i < outputMtxHeight; i++)
		{
			outputMtxData[i] = new unsigned char[outputMtxWidth];
			for (unsigned j = 0; j < outputMtxWidth; j++)
			{
				outputMtxData[i][j] = mtxData[i][j] + newMtxData[i][j] + 1;
			}
		}

		// 打开结果位图文件
		if (fopen_s(&file, destBmpName, "wb"))
		{
			cout << "Overlap output file open error!" << endl;
			return;
		}

		// 写入位图基本信息
		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fwrite(&outputBmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
		fwrite(clrTab, 1024, 1, file);

		// 写入转换结果
		for (unsigned i = 0; i < outputMtxHeight; i++)
		{
			fwrite(outputMtxData[i], 1, outputMtxWidth, file);
		}

		// 关闭文件
		fclose(file);

		// 释放无用内存
		for (unsigned i = 0; i < newMtxHeight; i++)
		{
			delete[] newMtxData[i];
			newMtxData[i] = nullptr;
		}

		for (unsigned i = 0; i < outputMtxHeight; i++)
		{
			delete[] outputMtxData[i];
			outputMtxData[i] = nullptr;
		}

		delete[] newMtxData;
		newMtxData = nullptr;

		delete[] outputMtxData;
		outputMtxData = nullptr;

		cout << "Overlap success" << endl;
	}
	catch (...)
	{
		cout << "Overlap failed" << endl;
	}
}