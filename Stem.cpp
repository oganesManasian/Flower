#include "Stem.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>

const float STEM_HEIGHT = 2.f;
const float STEM_HEIGHT0 = -2.f;
const float STEM_RADIUS = 0.2f;
const int STEM_POINTS_NUMBER = 20;
const XMFLOAT4 greenColor = XMFLOAT4(0.0, 1.0, 0.0, 1.0);

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

    // Create childs
    // ... TODO

    return hr;
}

void Stem::Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader) {
    XMMATRIX gWorld;

    // Update constant buffer
    pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);

    for (int i = 0; i < int(vTopology.size()); i++)
    {
        // Set vertex buffer
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        pImmediateContext->IASetVertexBuffers(0, 1, &vVertexBuffer.at(i), &stride, &offset);

        // Set index buffer
        pImmediateContext->IASetIndexBuffer(vIndexBuffer.at(i), DXGI_FORMAT_R16_UINT, 0);

        // Set primitive topology
        pImmediateContext->IASetPrimitiveTopology(vTopology.at(i));

        pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
        pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
        pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
        pImmediateContext->DrawIndexed(indicesNumber.at(i), 0, 0);
    }

    // Render childs
    for (Node* child : childs) {
        child->Render(cb, g_pVertexShader, g_pPixelShader);
    }
}

void Stem::Release(void) {
    // Release childs
    for (Node* child : childs) {
        child->Release();
    }

    // Self release (index, vertex buffers and so on)
    for (int i = 0; i < int(vTopology.size()); i++)
    {
        if (vVertexBuffer.at(i)) vVertexBuffer.at(i)->Release();
        if (vIndexBuffer.at(i)) vIndexBuffer.at(i)->Release();
    }

    //if (pVertexShader) pVertexShader->Release();
    //if (pPixelShader) pPixelShader->Release();
    // TODO Think how to release constant buffer
}