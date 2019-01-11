#include "Template.h"

class ManHattanTmp : public Template
{
public:
	char getOffsetX(unsigned i);
	char getOffsetY(unsigned i);
	float getWeight(unsigned i);
	unsigned getSize();

private:
	char m_OffX[13] = { 0, -1, 0, 1, -2, -1, 0, 1, 2, -1, 0, 1, 0 };
	char m_OffY[13] = { -2, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 2 };
	unsigned char m_Dis[13] = { 2, 2, 1, 2, 2, 1, 0, 1, 2, 2, 1, 2, 2 };
};