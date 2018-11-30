#pragma once
#include <Windows.h>
#include "Node.h"
#include "DataStructures.h"

class Stem : public Node {
public:
    HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext);
    virtual void Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader);
    void Release();
};