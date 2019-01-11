#include "ManHattanTmp.h"

char ManHattanTmp::getOffsetX(unsigned i)
{
	return m_OffX[i];
}

char ManHattanTmp::getOffsetY(unsigned i)
{
	return m_OffY[i];
}

float ManHattanTmp::getWeight(unsigned i)
{
	return m_Dis[i];
}

unsigned ManHattanTmp::getSize()
{
	return 13;
}
