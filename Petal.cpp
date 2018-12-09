#include "Petal.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>
#include "FlowerParameters.h"

#define DEG_TO_RAD(angle) (angle * 2 * PI / 360)

Petal::Petal(XMFLOAT3 shift, float angle, boolean isLower) {
    gWorld = XMMatrixIdentity();
    XMMATRIX rotation180 = XMMatrixRotationY(PI);
    gWorldBack = rotation180 * gWorld;

    if (isLower) {
        rotation = XMMatrixRotationY(-angle);
    }
    else {
        rotation = XMMatrixIdentity();
    }

    translation = XMMatrixTranslation(shift.x, shift.y, shift.z);

    this->isLower = isLower;
    return;
}

HRESULT Petal::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create trapeze
    std::vector<SimpleVertex> *vLowerVertices = new std::vector<SimpleVertex>();
    std::vector<WORD> *vLowerIndices = new std::vector<WORD>();

    float lowerWidth, upperWidth;
    if (isLower) {
        lowerWidth = PETAL_LOWEST_WIDTH;
        upperWidth = PETAL_BIGGEST_WIDTH;
    }
    else {
        lowerWidth = PETAL_BIGGEST_WIDTH;
        upperWidth = PETAL_LOWEST_WIDTH;
    }
    D3D_PRIMITIVE_TOPOLOGY topology = CreateTrapeze(vLowerVertices, vLowerIndices,
        PETAL_HEIGHT, lowerWidth, upperWidth, purpleColor);

    vTopology.push_back(topology);
    verticesNumber.push_back(int(vLowerVertices->size()));
    indicesNumber.push_back(int(vLowerIndices->size()));

    CreateVertexIndexConstantBuffers(vLowerVertices, vLowerIndices);

    return hr;
}

void Petal::ComputeWorldMatrix(float t) {
    FXMVECTOR slopeAxis = { 0.0, 0.0, 1.0 };
    XMMATRIX slope;

    if (isLower) {
        slope = XMMatrixRotationNormal(slopeAxis, -abs(sin(t)));
    }
    else {
        slope = XMMatrixRotationNormal(slopeAxis, abs(sin(t + PI/2)));
    }

    gWorld = slope * rotation * translation;
    XMMATRIX rotation180 = XMMatrixRotationY(PI);
    gWorldBack = rotation180 * gWorld;
}