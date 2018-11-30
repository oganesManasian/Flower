#pragma once
#include <Windows.h>
#include "Node.h"
#include "DataStructures.h"

class Stalk : public Node {
public:
    HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext);
    void Render(ConstantBuffer cb, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader);
    void Release();
};