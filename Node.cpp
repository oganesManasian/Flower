#include "Node.h"

HRESULT Node::CreateVertexIndexConstantBuffers(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices) {
    HRESULT hr = S_OK;

    int verticesNum = int(vertices->size());
    int indicesNum = int(indices->size());

    // Create vertex buffer
    
    ID3D11Buffer* vertexBuffer;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * verticesNum;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &(*vertices)[0];
    hr = pd3dDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);
    vVertexBuffer.push_back(vertexBuffer);
    if (FAILED(hr))
        return hr;

    // Create index buffer
    ID3D11Buffer* indexBuffer;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * indicesNum;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = &(*indices)[0];
    hr = pd3dDevice->CreateBuffer(&bd, &InitData, &indexBuffer);
    vIndexBuffer.push_back(indexBuffer);
    if (FAILED(hr))
        return hr;

    // Create the constant buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);
    if (FAILED(hr))
        return hr;
}

void Node::AddChild(Node* child) {
    childs.push_back(child);
}
