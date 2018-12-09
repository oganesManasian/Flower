#pragma once
#include <Windows.h>
#include "Node.h"
#include "DataStructures.h"

class Petal : public Node {
public:
    Petal(XMFLOAT3 centrePoint, float angle, boolean isLower);
    HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext);
    void ComputeWorldMatrix(float t);
private:
    XMMATRIX rotation;
    XMMATRIX translation;
    boolean isLower;
};