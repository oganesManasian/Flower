#include "Stalk.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>

const float STALK_HEIGHT = 0.1f;
const float STALK_HEIGHT0 = 0.f;
const float STALK_RADIUS = 0.7f;
const int STALK_POINTS_NUMBER = 20;
const XMFLOAT4 yellowColor = XMFLOAT4(1.0, 1.0, 0.0, 1.0);

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

    // Create childs
    // ... TODO

    return hr;
}

void Stalk::Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader) {
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

void Stalk::Release(void) {
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