#include "Stalk.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>
#include "FlowerParameters.h"

Stalk::Stalk(void) {
    gWorld = XMMatrixIdentity();
    XMMATRIX rotation180 = XMMatrixRotationY(PI);
    gWorldBack = rotation180 * gWorld;
}

HRESULT Stalk::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create cylinder
    std::vector<SimpleVertex> *vVerticesCylinder = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndicesCylinder = new std::vector<WORD>();
    D3D_PRIMITIVE_TOPOLOGY topology = CreateCylinder(vVerticesCylinder, vIndicesCylinder, 
        STALK_HEIGHT0, STALK_HEIGHT, STALK_RADIUS, STALK_POINTS_NUMBER, yellowColor);
    
    vTopology.push_back(topology);
    verticesNumber.push_back(int(vVerticesCylinder->size()));
    indicesNumber.push_back(int(vIndicesCylinder->size()));

    CreateVertexIndexConstantBuffers(vVerticesCylinder, vIndicesCylinder);

    // Create upper circle
    std::vector<SimpleVertex> *vVerticesUpperCircle = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndicesUpperCircle = new std::vector<WORD>();
    topology = CreateCircle(vVerticesUpperCircle, vIndicesUpperCircle,
        STALK_HEIGHT0 + STALK_HEIGHT, STALK_RADIUS, STALK_POINTS_NUMBER, yellowColor, true);

    vTopology.push_back(topology);
    verticesNumber.push_back(int(vVerticesUpperCircle->size()));
    indicesNumber.push_back(int(vIndicesUpperCircle->size()));

    CreateVertexIndexConstantBuffers(vVerticesUpperCircle, vIndicesUpperCircle);

    // Create lower circle
    std::vector<SimpleVertex> *vVerticesLowerCircle = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndicesLowerCircle = new std::vector<WORD>();
    topology = CreateCircle(vVerticesLowerCircle, vIndicesLowerCircle,
        STALK_HEIGHT0, STALK_RADIUS, STALK_POINTS_NUMBER, yellowColor, false);

    vTopology.push_back(topology);
    verticesNumber.push_back(int(vVerticesLowerCircle->size()));
    indicesNumber.push_back(int(vIndicesLowerCircle->size()));

    CreateVertexIndexConstantBuffers(vVerticesLowerCircle, vIndicesLowerCircle);

    return hr;
}

void Stalk::ComputeWorldMatrix(float t) {
    t;
}