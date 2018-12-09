#include "Flower.h"


HRESULT Flower::Create(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext) {
    HRESULT hr = S_OK;

    pd3dDevice = g_pd3dDevice;
    pImmediateContext = g_pImmediateContext;

    // Create stem
    root = new Stem();
    root->Init(NULL, g_pd3dDevice, g_pImmediateContext);
    
    // Create stalk
    Node* stalk = new Stalk();
    stalk->Init(root, g_pd3dDevice, g_pImmediateContext);
    root->AddChild(stalk);
    
    // Create petals
    float angleStep = 2 * PI / PETAL_NUMBER;
    float curAngle = 0.f;
    
    for (int i = 0; i < PETAL_NUMBER; ++i, curAngle += angleStep) {
        float x = STALK_RADIUS * cos(curAngle);
        float z = STALK_RADIUS * sin(curAngle);
        float y = PETAL_HEIGHT0;
    
        // Lower petal
        boolean isLower = true;
        XMFLOAT3 shiftLower = XMFLOAT3(x, y, z);
        Node* lowerPetal = new Petal(shiftLower, curAngle, isLower);
        lowerPetal->Init(stalk, g_pd3dDevice, g_pImmediateContext);
        stalk->AddChild(lowerPetal);
    
        //// Upper petal
        isLower = false;
        XMFLOAT3 shiftUpper = XMFLOAT3(0.f, PETAL_HEIGHT, 0.f);
        Node* upperPetal = new Petal(shiftUpper, curAngle, isLower);
        upperPetal->Init(lowerPetal, g_pd3dDevice, g_pImmediateContext);
        lowerPetal->AddChild(upperPetal);
    }

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

    root->Render(cb1, g_pVertexShader, g_pPixelShader, t);
}

void Flower::Release(void) {
    root->Release();
}