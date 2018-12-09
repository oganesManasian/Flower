#pragma once
#include "Node.h"
#include "Stem.h"
#include "Stalk.h"
#include "Petal.h"
#include "FlowerParameters.h"

class Flower {
private:
    Node* root = nullptr;
    //ID3D11Buffer* pConstantBuffer = nullptr;
    ID3D11Device* pd3dDevice = nullptr;
    ID3D11DeviceContext* pImmediateContext = nullptr;
public:
    HRESULT Create(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext);;
    void Render(float t, XMMATRIX g_View, XMMATRIX g_Projection, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader);
    void Release(void);
};
