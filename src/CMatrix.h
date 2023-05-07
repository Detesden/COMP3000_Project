#pragma once

#include "CVector.h"


struct CMatrix
{
	float data[16];
	//float data[4][4];
};



class CMatrix3x4
{
public:
    float data[3][4];

    const float* operator[](size_t index) const
    {
        return data[index];
    }

    CVector Origin() const
    {
        return { data[0][3], data[1][3], data[2][3] };
    }
};