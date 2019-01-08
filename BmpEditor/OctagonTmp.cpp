#include "OctagonTmp.h"

char OctagonTmp::getOffsetX(unsigned i)
{
	return m_OffsetX[i];
}

char OctagonTmp::getOffsetY(unsigned i)
{
	return m_OffsetY[i];
}

float OctagonTmp::getWeight(unsigned i)
{
	return m_Dis[i];
}

unsigned OctagonTmp::getSize()
{
	return 37;
}