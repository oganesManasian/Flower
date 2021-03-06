#pragma once
#include <Windows.h>
#include "Node.h"
#include "DataStructures.h"

class Stem : public Node {
public:
    Stem(void);
    HRESULT Init(Node* parent, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pImmediateContext);
    void ComputeWorldMatrix(float t);
};