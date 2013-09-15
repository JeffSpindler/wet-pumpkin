// Geometric Tools, LLC
// Copyright (c) 1998-2013
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef APPMAINWM5_H
#define APPMAINWM5_H

#include "Geom3d.h"

#include "Wm5WindowApplication3.h"
using namespace Wm5;

class CrunchApp;
class CommGeom3dClient;
class CommGeom3dServer;

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
	void print_help();

	// App Vars
	CrunchApp *m_app;
	std::string m_name;
    CommGeom3dClient *m_comm_client;
    CommGeom3dServer *m_comm_server;
	Geom3d_dq_t m_comm_g3d_dq;
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
