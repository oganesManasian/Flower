#include "Stalk.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>

const float STEM_HEIGHT = 0.1f;
const float STEM_HEIGHT0 = 0.f;
const float STEM_RADIUS = 1.f;
const int STEM_POINTS_NUMBER = 10;
const XMFLOAT4 yellowColor = XMFLOAT4(1.0, 1.0, 0.0, 1.0);

HRESULT Stalk::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create figure
    std::vector<SimpleVertex> *vVerticesCylinder = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndicesCylinder = new std::vector<WORD>();
    D3D_PRIMITIVE_TOPOLOGY topology = CreateCylinder(vVerticesCylinder, vIndicesCylinder, STEM_HEIGHT0, STEM_HEIGHT, STEM_RADIUS, STEM_POINTS_NUMBER, yellowColor);
    vTopology.push_back(topology);
    indicesNumber.push_back(vIndicesCylinder->size());
    verticesNumber.push_back(vVerticesCylinder->size());

    // Create vertex buffer
    ID3D11Buffer* vertexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * verticesNumber.at(0);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &(*vVerticesCylinder)[0];
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);
    vVertexBuffer.push_back(vertexBuffer);
    if (FAILED(hr))
        return hr;

    // Create index buffer
    ID3D11Buffer* indexBuffer;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indicesNumber.at(0);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = &(*vIndicesCylinder)[0];
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);
    vIndexBuffer.push_back(indexBuffer);
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