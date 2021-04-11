#pragma once

#ifndef _DX11BASE_H_
#define _DX11BASE_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <SimpleMath.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

class dX11Base
{
    // Constructors
public:
    dX11Base();
    virtual ~dX11Base();

    // Methods
public:
    bool Initialize(HWND hWnd, HINSTANCE hInst);
    bool CompileShader(LPCWSTR szFilePath, LPCSTR szFunc, LPCSTR szShaderModel, ID3DBlob** buffer);

    // Overrides
public:
    virtual bool LoadContent() = 0;

    virtual void Update() = 0;
    virtual void Render() = 0;

    // Attributes
public:
    HWND m_hWnd;
    HINSTANCE m_hInst;
    ID3D11Device* m_pD3DDevice;
    ID3D11DeviceContext* m_pD3DContext;
    ID3D11RenderTargetView* m_pD3DRenderTargetView;
    IDXGISwapChain* m_pSwapChain;

};

#endif // !_DX11BASE_H
