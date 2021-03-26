#include "BlankScene.h"

BlankScene::BlankScene()
{
}

BlankScene::~BlankScene()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides

bool BlankScene::LoadContent()
{
    return true;
}

void BlankScene::UnloadContent()
{
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
    float color[4] = { 0.1f, 0.2f, 0.5f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);

    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}

