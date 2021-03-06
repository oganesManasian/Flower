#pragma once
#include <vector>
#include <d3d11_1.h>
#include "DataStructures.h"

class Node {
public:
    virtual HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) = 0;
    void Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader, float t);
    void Release();
    virtual void ComputeWorldMatrix(float t) = 0;
    void AddChild(Node* child);
protected:
    XMMATRIX gWorld;
    XMMATRIX gWorldBack;
    std::vector<Node*> childs;
    Node* parent;
    ID3D11DeviceContext* pImmediateContext = nullptr;
    ID3D11Device* pd3dDevice = nullptr;
    std::vector<ID3D11Buffer*> vVertexBuffer;
    std::vector<ID3D11Buffer*> vIndexBuffer;
    std::vector<D3D_PRIMITIVE_TOPOLOGY> vTopology;
    std::vector<int> verticesNumber;
    std::vector<int> indicesNumber;
    ID3D11Buffer* pConstantBuffer = nullptr;
    HRESULT CreateVertexIndexConstantBuffers(std::vector<SimpleVertex> *vertices, std::vector<WORD> *indices);
};