#pragma once
#include <vector>
#include <d3d11_1.h>
#include "DataStructures.h"

class Node {
public:
    virtual HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) = 0;
    virtual void Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader) = 0;
    virtual void Release() = 0;
//protected:
    std::vector<Node*> childs;
    Node* parent;
    ID3D11DeviceContext* pImmediateContext = nullptr;
    ID3D11Device* pd3dDevice = nullptr;
    ID3D11Buffer* pVertexBuffer = nullptr;
    ID3D11Buffer* pIndexBuffer = nullptr;
    ID3D11Buffer* pConstantBuffer = nullptr;
    int verticesNumber;
    int indicesNumber;
};