#pragma once
#include <directxmath.h>

using namespace DirectX;
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};
