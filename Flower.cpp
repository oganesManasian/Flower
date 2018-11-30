#include "Flower.h"

HRESULT Flower::Create(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;

    root = new Stem();
    root->Init(NULL, g_pd3dDevice, g_pImmediateContext);

    return hr;
}

void Flower::Render(float t, XMMATRIX g_View, XMMATRIX g_Projection,
    ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader) {

    // 1st Cube: Rotate around the origin
    XMMATRIX g_World1;
    g_World1 = XMMatrixRotationY(t);

    //
    // Update variables for the first figure
    //
    ConstantBuffer cb1;
    cb1.mWorld = XMMatrixTranspose(g_World1);
    cb1.mView = XMMatrixTranspose(g_View);
    cb1.mProjection = XMMatrixTranspose(g_Projection);

    root->Render(cb1, g_pVertexShader, g_pPixelShader);
}

void Flower::Release(void) {
    root->Release();
}