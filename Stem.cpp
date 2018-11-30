#include "Stem.h"
#include <d3d11_1.h>
#include "Figures.h"
#include <vector>

const float STEM_HEIGHT = 2.f;
const float STEM_HEIGHT0 = -2.f;
const float STEM_RADIUS = 0.5f;
const int STEM_POINTS_NUMBER = 10;
const XMFLOAT4 greenColor = XMFLOAT4(0.0, 1.0, 0.0, 1.0);

HRESULT Stem::Init(Node* parentNode, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;
    parent = parentNode;

    // Create figure
    std::vector<SimpleVertex> *vVertices = new std::vector<SimpleVertex>();
    std::vector<WORD> *vIndices = new std::vector<WORD>();
    D3D_PRIMITIVE_TOPOLOGY topology = CreateCylinder(vVertices, vIndices, STEM_HEIGHT0, STEM_HEIGHT, STEM_RADIUS, STEM_POINTS_NUMBER, greenColor);
    vTopology.push_back(topology);
    indicesNumber.push_back(vIndices->size());
    verticesNumber.push_back(vVertices->size());

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
    InitData.pSysMem = &(*vVertices)[0];
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
    InitData.pSysMem = &(*vIndices)[0];
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
    /*
    // Vertices' buffer
    float height = 0.0f;
    SimpleVertex vertices[VERTICES_NUMBER] = {
        {XMFLOAT3(0.0, 0.0, height), XMFLOAT4(1.0, 0.0, 0.0, 1.0)},
        {XMFLOAT3(1.0, 0.0, height), XMFLOAT4(0.0, 1.0, 0.0, 1.0)},
        {XMFLOAT3(0.0, 1.0, height), XMFLOAT4(0.0, 0.0, 1.0, 1.0)},
        {XMFLOAT3(1.0, 1.0, height), XMFLOAT4(1.0, 1.0, 1.0, 1.0)},
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * VERTICES_NUMBER;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
    if (FAILED(hr))
        return hr;

    // Set primitive topology
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Indices' buffer
    WORD indices[INDICES_NUMBER] = {
        2, 0, 3,
        0, 1, 3
    };

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * INDICES_NUMBER;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
    if (FAILED(hr))
        return hr;
    return hr;
    */
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