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
    virtual void UnloadContent();

    virtual void Update();
    virtual void Render();

};

#endif // !_BLANKSCENE_H_