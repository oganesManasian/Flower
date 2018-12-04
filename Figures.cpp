#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include <map>
#include "resource.h"
#include "Figures.h"
#include "DataStructures.h"

using namespace DirectX;

const static float PI = 3.14159;

XMFLOAT3 XMFloat3GetMiddlePoint(XMFLOAT3 a, XMFLOAT3 b)
{
    return XMFLOAT3((a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2);
}

XMFLOAT3 XMFloat3Normalize(XMFLOAT3 a)
{
    float r = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    return XMFLOAT3(a.x / r, a.y / r, a.z / r);
}

D3D_PRIMITIVE_TOPOLOGY CreateCylinder(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices,
    float z0, float height, float radius, int pointsNumber, XMFLOAT4 color)
{
    float angleStep = 2 * PI / pointsNumber;
    float curAngle = 0.f;

    D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    for (int i = 0; i < pointsNumber + 1; ++i, curAngle += angleStep)
    {
        float x = radius * cos(curAngle);
        float y = radius * sin(curAngle);

        vertices->push_back({ XMFLOAT3(x, z0,          y), color });
        vertices->push_back({ XMFLOAT3(x, z0 + height, y), color });
    }

    int shift = 2;
    int curInd = 0;
    for (int i = 0; i < pointsNumber; ++i, curInd += shift)
    {
        // Upper triangle
        indices->push_back(WORD(curInd));
        indices->push_back(WORD(curInd + 1));
        indices->push_back(WORD(curInd + 2));

        // Lower triangle
        indices->push_back(WORD(curInd + 1));
        indices->push_back(WORD(curInd + 3));
        indices->push_back(WORD(curInd + 2));
    }
    // Last trinagles
    indices->push_back(WORD(curInd));
    indices->push_back(WORD(curInd + 1));
    indices->push_back(WORD(1));

    indices->push_back(WORD(curInd + 1));
    indices->push_back(WORD(1));
    indices->push_back(WORD(0));

    return topology;
}

D3D_PRIMITIVE_TOPOLOGY CreateCircle(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices,
    float z0, float radius, int pointsNumber, XMFLOAT4 color, bool isLookingFromAbove) {
    float angleStep = 2 * PI / pointsNumber;
    float curAngle = 0.f;
    D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // Centr point
    vertices->push_back({ XMFLOAT3(0.f, z0, 0.f), color });

    for (int i = 0; i < pointsNumber + 1; ++i, curAngle += angleStep)
    {
        float x = radius * cos(curAngle);
        float y = radius * sin(curAngle);

        vertices->push_back({ XMFLOAT3(x, z0, y), color });
    }

    int shift = 1;
    for (int i = 0; i < pointsNumber - 1; i += shift)
    {
        indices->push_back(WORD(0));
        if (isLookingFromAbove)
        {
            indices->push_back(WORD(i + 2));
            indices->push_back(WORD(i + 1));
        }
        else
        {
            indices->push_back(WORD(i + 1));
            indices->push_back(WORD(i + 2));
        }
    }

    // Last triangle
    indices->push_back(WORD(0));
    if (isLookingFromAbove)
    {
        indices->push_back(WORD(1));
        indices->push_back(WORD(pointsNumber));
    }
    else
    {
        indices->push_back(WORD(pointsNumber));
        indices->push_back(WORD(1));
    }
    return topology;
}

D3D_PRIMITIVE_TOPOLOGY CreateTrapeze(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices,
    XMFLOAT3 centrPoint, XMFLOAT3 norm, float height, float lowerWidth, float upperWidth, XMFLOAT4 color, bool isUpsideDown) {
    D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    XMFLOAT3 leftLower = XMFLOAT3(0.f, -height / 2, -lowerWidth / 2);
    XMFLOAT3 leftUpper = XMFLOAT3(0.f, height / 2, -upperWidth / 2);
    XMFLOAT3 rightUpper = XMFLOAT3(0.f, height / 2, upperWidth / 2);
    XMFLOAT3 rightLower = XMFLOAT3(0.f, -height / 2, lowerWidth / 2);
    vertices->push_back({ leftLower, color });
    vertices->push_back({ leftUpper, color });
    vertices->push_back({ rightUpper, color });
    vertices->push_back({ rightLower, color });

    // Upper triangle
    indices->push_back(0);
    indices->push_back(1);
    indices->push_back(2);

    // Lower triangle
    indices->push_back(0);
    indices->push_back(2);
    indices->push_back(3);

    return topology;
}