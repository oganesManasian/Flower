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

    if (!pConstantBuffer) {
        // Create the constant buffer
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBuffer);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        hr = pd3dDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);
        if (FAILED(hr))
            return hr;
    }

    return hr;
}

void Node::Render(ConstantBuffer cb, ID3D11VertexShader* g_pVertexShader, ID3D11PixelShader* g_pPixelShader, float t) {
    ComputeWorldMatrix(t);
    XMMATRIX worldMatrices[2] = {
        cb.mWorld * XMMatrixTranspose(gWorld),
        cb.mWorld * XMMatrixTranspose(gWorldBack)
    };

    for (XMMATRIX world : worldMatrices) {
        // Update constant buffer
        cb.mWorld = world;
        pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);

        for (int i = 0; i < int(vTopology.size()); i++)
        {
            // Set vertex buffer
            UINT stride = sizeof(SimpleVertex);
            UINT offset = 0;
            pImmediateContext->IASetVertexBuffers(0, 1, &vVertexBuffer.at(i), &stride, &offset);

            // Set index buffer
            pImmediateContext->IASetIndexBuffer(vIndexBuffer.at(i), DXGI_FORMAT_R16_UINT, 0);

            // Set primitive topology
            pImmediateContext->IASetPrimitiveTopology(vTopology.at(i));

            pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
            pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
            pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
            pImmediateContext->DrawIndexed(indicesNumber.at(i), 0, 0);
        }
    }

    // Render childs
    cb.mWorld = worldMatrices[0];
    for (Node* child : childs) {
        child->Render(cb, g_pVertexShader, g_pPixelShader, t);
    }
}

void Node::AddChild(Node* child) {
    childs.push_back(child);
}

void Node::Release(void) {
    // Release childs
    for (Node* child : childs) {
        child->Release();
    }

    // Self release (index, vertex buffers and so on)
    for (int i = 0; i < int(vTopology.size()); i++)
    {
        if (vVertexBuffer.at(i)) vVertexBuffer.at(i)->Release();
        if (vIndexBuffer.at(i)) vIndexBuffer.at(i)->Release();
    }
    pConstantBuffer->Release();
}