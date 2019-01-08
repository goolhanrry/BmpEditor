class OctagonTmp
{
public:
	char getOffsetX(unsigned i);
	char getOffsetY(unsigned i);
	float getWeight(unsigned i);
	unsigned getSize();

private:
	char m_OffsetX[37] = { 1, 0, -1, 2, 1, 0, -1, -2, 3, 2, 1, 0, -1, -2, -3, -3, -2, -1, 0, 1, 2, 3, 3, 2, 1, 0, -1, -2, -3, 2, 1, 0, -1, -2, 1, 0, -1 };
	char m_OffsetY[37] = { -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3 };
	unsigned char m_Dis[37] = { 3, 3, 3, 3, 2, 2, 2, 3, 3, 2, 2, 1, 2, 2, 3, 3, 2, 1, 0, 1, 2, 3, 3, 2, 2, 1, 2, 2, 3, 3, 2, 2, 2, 3, 3, 3, 3 };
};