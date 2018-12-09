#include "Stem.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>
#include "FlowerParameters.h"

Stem::Stem(void) {
    gWorld = XMMatrixIdentity();
    XMMATRIX rotation180 = XMMatrixRotationY(PI);
    gWorldBack = rotation180 * gWorld;
}

HRESULT Stem::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create cylinder
    std::vector<SimpleVertex> *vVertices = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndices = new std::vector<WORD>();
    D3D_PRIMITIVE_TOPOLOGY topology = CreateCylinder(vVertices, vIndices, 
        STEM_HEIGHT0, STEM_HEIGHT, STEM_RADIUS, STEM_POINTS_NUMBER, greenColor);
    
    vTopology.push_back(topology);
    indicesNumber.push_back(int(vIndices->size()));
    verticesNumber.push_back(int(vVertices->size()));

    CreateVertexIndexConstantBuffers(vVertices, vIndices);

    // Create lower circle
    std::vector<SimpleVertex> *vVerticesLowerCircle = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndicesLowerCircle = new std::vector<WORD>();
    topology = CreateCircle(vVerticesLowerCircle, vIndicesLowerCircle,
        STEM_HEIGHT0, STEM_RADIUS, STEM_POINTS_NUMBER, greenColor, false);

    vTopology.push_back(topology);
    verticesNumber.push_back(int(vVerticesLowerCircle->size()));
    indicesNumber.push_back(int(vIndicesLowerCircle->size()));

    CreateVertexIndexConstantBuffers(vVerticesLowerCircle, vIndicesLowerCircle);

    return hr;
}

void Stem::ComputeWorldMatrix(float t) {
    t;
}
