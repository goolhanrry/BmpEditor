#include "AverageTmp.h"

char AverageTmp::getOffsetX(unsigned i)
{
	return m_OffsetX[i];
}

char AverageTmp::getOffsetY(unsigned i)
{
	return m_OffsetY[i];
}

float AverageTmp::getWeight(unsigned i)
{
	return m_Weight[i];
}

unsigned AverageTmp::getSize()
{
	return 9;
}