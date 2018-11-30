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

//--------------------------------------------------------------------------------------
// Recursive sphere creation
// Starting figure is octahedron
//--------------------------------------------------------------------------------------
void CreateSphere(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices, int recursionLevel)
{
    float t = float((1 + sqrt(5)) / 2);
    std::vector<SimpleVertex> vVertices0 = {
        { XMFLOAT3(-1.0f, t, 0), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, t, 0), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -t, 0), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -t, 0), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },

        { XMFLOAT3(0, -1.0f, t), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0, 1.0f, t), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(0, -1.0f, -t), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0, 1.0f, -t), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

        { XMFLOAT3(t, 0, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(t, 0, 1.0f), XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-t, 0, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-t, 0, 1.0f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f) },
    };

    // Normalize baze points
    std::vector<SimpleVertex> vVertices;
    for (auto vert0 : vVertices0)
    {
        vVertices.push_back({ XMFloat3Normalize(vert0.Pos), vert0.Color });
    }

    // Create index buffer
    std::vector<WORD> vIndices =
    {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1,
    };

    // Refine triangles
    for (int level = 0; level < recursionLevel; level++)
    {
        std::vector<WORD> vNewIndices;
        int vertN = vVertices.size();
        int indN = vIndices.size();
        for (int i = 0; i < indN; i += 3)
        {
            // Divide triangle on four ones
            XMFLOAT3 a = XMFloat3GetMiddlePoint(vVertices.at(vIndices.at(i)).Pos, vVertices.at(vIndices.at(i + 1)).Pos);
            XMFLOAT3 b = XMFloat3GetMiddlePoint(vVertices.at(vIndices.at(i + 1)).Pos, vVertices.at(vIndices.at(i + 2)).Pos);
            XMFLOAT3 c = XMFloat3GetMiddlePoint(vVertices.at(vIndices.at(i + 2)).Pos, vVertices.at(vIndices.at(i)).Pos);
            // Normalize points
            a = XMFloat3Normalize(a);
            b = XMFloat3Normalize(b);
            c = XMFloat3Normalize(c);
            // Add points
            vVertices.push_back({ a, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) });
            vVertices.push_back({ b, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });
            vVertices.push_back({ c, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
            // Add triangles
            unsigned short indA = unsigned short(vertN + i);
            unsigned short indB = unsigned short(vertN + i + 1);
            unsigned short indC = unsigned short(vertN + i + 2);

            std::vector<WORD> vTri1 = { vIndices.at(unsigned short(i)), indA, indC };
            vNewIndices.insert(std::end(vNewIndices), std::begin(vTri1), std::end(vTri1));

            std::vector<WORD> vTri2 = { indA, vIndices.at(unsigned short(i + 1)), indB };
            vNewIndices.insert(std::end(vNewIndices), std::begin(vTri2), std::end(vTri2));

            std::vector<WORD> vTri3 = { indC, indB, vIndices.at(unsigned short(i + 2)) };
            vNewIndices.insert(std::end(vNewIndices), std::begin(vTri3), std::end(vTri3));

            std::vector<WORD> vTri4 = { indA, indB, indC };
            vNewIndices.insert(std::end(vNewIndices), std::begin(vTri4), std::end(vTri4));
        }
        vIndices.clear();
        vIndices.insert(std::begin(vIndices), std::begin(vNewIndices), std::end(vNewIndices));
    }

    // Copy vertices in constant buffer
    for (int i = 0; i < int(vVertices.size()); ++i)
        (*vertices).push_back(vVertices.at(i));

    // Copy indices in constant buffer
    for (int i = 0; i < int(vIndices.size()); ++i)
        (*indices).push_back(vIndices.at(i));
    return;
}

void CreateCylinder(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices, 
    float z0, float height, float radius, int pointsNumber, XMFLOAT4 color)
{
    float angleStep = 2 * PI / pointsNumber;
    float curAngle = 0.f;
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
}