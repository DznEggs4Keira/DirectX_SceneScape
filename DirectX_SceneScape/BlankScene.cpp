//////////////////////////////////////////////////////////////////////////////////
//// @TODO This Class will become the Scene Manager Class which will handle  ////
//// which Scene will be loaded and when.                                   ////
///////////////////////////////////////////////////////////////////////////////

#include "BlankScene.h"

using namespace SimpleMath;

// Vertex struct
struct Vertex
{
    Vector3 pos;
    Vector2 tex0;
};

//////////////////////////////////////////////////////////////////////
// Constructors

BlankScene::BlankScene()
{
    m_pVSGreenColor = NULL;
    m_pPSGreenColor = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;

    m_pColorMap = NULL;
    m_pColorMapSampler = NULL;
}

BlankScene::~BlankScene()
{
    // Cleanup
    if (m_pColorMap)
        m_pColorMap->Release();
    m_pColorMap = NULL;
    if (m_pColorMapSampler)
        m_pColorMapSampler->Release();
    m_pColorMapSampler = NULL;

    if (m_pVSGreenColor)
        m_pVSGreenColor->Release();
    m_pVSGreenColor = NULL;
    if (m_pPSGreenColor)
        m_pPSGreenColor->Release();
    m_pPSGreenColor = NULL;
    if (m_pInputLayout)
        m_pInputLayout->Release();
    m_pInputLayout = NULL;
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
}

//////////////////////////////////////////////////////////////////////
// Overrides

bool BlankScene::LoadContent()
{
    // Compile vertex shader
    ID3DBlob* pVSBuffer = NULL;
    bool res = CompileShader(L"TextureShader.fx", "main", "vs_4_0", &pVSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load vertex shader", L"ERROR", MB_OK);
        return false;
    }

    // Create vertex shader
    HRESULT hr;
    hr = m_pD3DDevice->CreateVertexShader(
        pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(),
        0, &m_pVSGreenColor);
    if (FAILED(hr)) {
        if (pVSBuffer)
            pVSBuffer->Release();
        return false;
    }

    // Define input layout
    D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

    // Create input layout
    hr = m_pD3DDevice->CreateInputLayout(
        shaderInputLayout, numLayoutElements,
        pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(),
        &m_pInputLayout);
    if (FAILED(hr)) {
        return false;
    }

    // Release VS buffer
    pVSBuffer->Release();
    pVSBuffer = NULL;

    // Compile pixel shader
    ID3DBlob* pPSBuffer = NULL;
    res = CompileShader(L"TextureShader.fx", "PS_Main", "ps_4_0", &pPSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load pixel shader", L"ERROR", MB_OK);
        return false;
    }

    // Create pixel shader
    hr = m_pD3DDevice->CreatePixelShader(
        pPSBuffer->GetBufferPointer(),
        pPSBuffer->GetBufferSize(),
        0, &m_pPSGreenColor);
    if (FAILED(hr)) {
        return false;
    }

    // Cleanup PS buffer
    pPSBuffer->Release();
    pPSBuffer = NULL;

    // Define triangle
    /*Vertex vertices[] =
    {
        Vector3(0.0f,  0.5f, 0.5f),
        Vector3(0.5f, -0.5f, 0.5f),
        Vector3(-0.5f, -0.5f, 0.5f)
    };*/

    // Define Square
    Vertex vertices[] =
    {
        {Vector3(-0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)}, // Bottom Left
        {Vector3(-0.5f, 0.5f, 0.0f), Vector2(1.0f, 0.0f)},// Top Left
        {Vector3(0.5f, 0.5f, 0.0f), Vector2(0.0f, 0.0f)},// Top Right
        
        {Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 0.0f)}, // Bottom Left
        {Vector3(0.5f, 0.5f, 0.0f), Vector2(0.0f, 1.0f)}, // Top Right
        {Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)}  // Bottom Right.
        
    };

    // Define Star
    /*Vertex vertices[] =
    {
        Vector3(-0.3f, 0.0f, 0.0f), // Left
        Vector3(0.0f, 0.3f, 0.0f), // Top
        Vector3(0.3f, 0.0f, 0.0f), // Right
        Vector3(-0.3f, 0.2f, 0.0f), // Invert Left
        Vector3(0.3f, 0.2f, 0.0f), // Invert Right
        Vector3(0.0f, -0.2f, 0.0f)  // Bottom.

    };*/

    // Vertex description
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 6;

    // Resource data
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));
    resourceData.pSysMem = vertices;

    // Create vertex buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Load texture
    hr = ::CreateDDSTextureFromFile(
        m_pD3DDevice, L"borg.dds", NULL, &m_pColorMap);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to load texture", L"ERROR", MB_OK);
        return false;
    }

    // Texture sampler
    D3D11_SAMPLER_DESC textureDesc;
    ::ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textureDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textureDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textureDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = m_pD3DDevice->CreateSamplerState(&textureDesc, &m_pColorMapSampler);
    if (FAILED(hr)) {
        ::MessageBox(m_hWnd, L"Unable to create texture sampler state", L"ERROR", MB_OK);
        return false;
    }


    return true;
}

void BlankScene::Update()
{
}

void BlankScene::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.0f, 0.5f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);


    // Stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set vertex buffer
    m_pD3DContext->IASetInputLayout(m_pInputLayout);
    m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    m_pD3DContext->VSSetShader(m_pVSGreenColor, 0, 0);
    m_pD3DContext->PSSetShader(m_pPSGreenColor, 0, 0);
    m_pD3DContext->PSSetShaderResources(0, 1, &m_pColorMap);
    m_pD3DContext->PSSetSamplers(0, 1, &m_pColorMapSampler);

    // Draw triangle
    m_pD3DContext->Draw(6, 0);

    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}

