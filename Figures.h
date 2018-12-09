#pragma once
#include "DataStructures.h"

D3D_PRIMITIVE_TOPOLOGY CreateCylinder(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices, 
    float z0, float height, float radius, int pointsNumber, XMFLOAT4 color);

D3D_PRIMITIVE_TOPOLOGY CreateCircle(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices,
    float z0, float radius, int pointsNumber, XMFLOAT4 color, bool isLookingFromAbove);

D3D_PRIMITIVE_TOPOLOGY CreateTrapeze(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices,
    float height, float lowerWidth, float upperWidth, XMFLOAT4 color);