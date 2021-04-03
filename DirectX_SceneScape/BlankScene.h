#pragma once

#ifndef _BLANKSCENE_H_

#define _BLANKSCENE_H_

#include "dX11Base.h"

class BlankScene :
    public dX11Base
{
    // Constructors
public:
    BlankScene();
    virtual ~BlankScene();

    // Overrides
public:
    virtual bool LoadContent();

    virtual void Update();
    virtual void Render();

    // Members
protected:
    ID3D11VertexShader* m_pVSGreenColor;
    ID3D11PixelShader* m_pPSGreenColor;
    ID3D11InputLayout* m_pInputLayout;
    ID3D11Buffer* m_pVertexBuffer;

    ID3D11ShaderResourceView* m_pColorMap;
    ID3D11SamplerState* m_pColorMapSampler;
};

#endif // !_BLANKSCENE_H_