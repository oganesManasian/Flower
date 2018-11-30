#pragma once
#include "DataStructures.h"

void CreateSphere(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices, int recursionLevel);
void CreateCylinder(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices, 
    float z0, float height, float radius, int pointsNumber, XMFLOAT4 color);