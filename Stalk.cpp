#include "Stalk.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>

const float STEM_HEIGHT = 0.1f;
const float STEM_HEIGHT0 = 0.f;
const float STEM_RADIUS = 1.f;
const int STEM_POINTS_NUMBER = 10;

HRESULT Stalk::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create figure
    std::vector<SimpleVertex> *vVertices = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndices = new std::vector<WORD>();
    CreateCylinder(vVertices, vIndices, STEM_HEIGHT0, STEM_HEIGHT, STEM_RADIUS, STEM_POINTS_NUMBER, XMFLOAT4(0.0, 1.0, 0.0, 1.0));
    //CreateSphere(vVertices, vIndices, RECURSION_LEVEL);
    indicesNumber = vIndices->size();
    verticesNumber = vVertices->size();

    // Create vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * verticesNumber;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &(*vVertices)[0];
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
    if (FAILED(hr))
        return hr;

    // Create index buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indicesNumber;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = &(*vIndices)[0];
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
    if (FAILED(hr))
        return hr;

    // Create the constant buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);
    if (FAILED(hr))
        return hr;

    // Create childs
    // ... TODO

    return hr;
}

void Stalk::Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader) {
    XMMATRIX gWorld;

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

    // Set index buffer
    pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // Set primitive topology
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
    pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
    pImmediateContext->DrawIndexed(indicesNumber, 0, 0);
}

void Stalk::Release(void) {
    // Release childs
    // ...

    // Self release (index, vertex buffers and so on)
    // ...
    if (pVertexBuffer) pVertexBuffer->Release();
    if (pIndexBuffer) pIndexBuffer->Release();
    //if (pVertexShader) pVertexShader->Release();
    //if (pPixelShader) pPixelShader->Release();
    // TODO Think how to release constant buffer
}