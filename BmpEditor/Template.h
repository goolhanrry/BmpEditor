#pragma once
class Template
{
public:
	virtual char getOffsetX(unsigned i) = 0;
	virtual char getOffsetY(unsigned i) = 0;
	virtual float getWeight(unsigned i) = 0;
	virtual unsigned getSize() = 0;
};