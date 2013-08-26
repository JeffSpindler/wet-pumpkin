// Geometric Tools, LLC
// Copyright (c) 1998-2013
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef APPMAINWM5_H
#define APPMAINWM5_H

#include "Wm5WindowApplication3.h"
using namespace Wm5;

class AppMainWm5 : public WindowApplication3
{
    WM5_DECLARE_INITIALIZE;
    WM5_DECLARE_TERMINATE;

public:
    AppMainWm5 ();

    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();
    virtual bool OnKeyDown (unsigned char key, int x, int y);

protected:

    void CreateScene ();

    void PhysicsTick ();
    void GraphicsTick ();


    // Simulated clock.
    float mSimTime, mSimDeltaTime;

    // The scene graph.
    NodePtr mScene;
    WireStatePtr mWireState;
    Culler mCuller;

    Float4 mTextColor;
};

WM5_REGISTER_INITIALIZE(AppMainWm5);
WM5_REGISTER_TERMINATE(AppMainWm5);

#endif
