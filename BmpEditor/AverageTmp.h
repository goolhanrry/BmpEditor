#include "Template.h"

class AverageTmp : public Template
{
public:
	char getOffsetX(unsigned i);
	char getOffsetY(unsigned i);
	float getWeight(unsigned i);
	unsigned getSize();

private:
	char m_OffsetX[9] = { 1, 0, -1, -1, 0, 1, 1, 0, -1 };
	char m_OffsetY[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	char m_Weight[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
};