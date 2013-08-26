// Perform3-D LLC
// Copyright (c) 2013
//
//
// File Version: 1.0.0 (2013/08/26)

#include "AppMainWm5.h"

WM5_WINDOW_APPLICATION(AppMainWm5);

//----------------------------------------------------------------------------
AppMainWm5::AppMainWm5 ()
    :
    WindowApplication3("AppMainWm5", 0, 0, 640, 480,
        Float4(1.0f, 1.0f, 1.0f, 1.0f)),
        mTextColor(0.0f, 0.0f, 0.0f, 1.0f)
{
    mSimTime = 0.0f;
    mSimDeltaTime = 1.0f/10.0f;
}
//----------------------------------------------------------------------------
bool AppMainWm5::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }

	// Init app


	// Init WM5 Window
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 1.0f, 1000.0f);
    float angle = 0.02f*Mathf::PI;
    float cs = Mathf::Cos(angle), sn = Mathf::Sin(angle);
    APoint camPosition(27.5f, 8.0f, 8.9f);
    AVector camDVector(-cs, 0.0f, -sn);
    AVector camUVector(-sn, 0.0f, cs);
    AVector camRVector = camDVector.Cross(camUVector);
    mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);

    CreateScene();
    // Initial update of objects.
    mScene->Update();

    // Initialize objects with correct transformations.
    PhysicsTick();

    // Initial culling of scene.
    mCuller.SetCamera(mCamera);
    mCuller.ComputeVisibleSet(mScene);
    return true;
}
//----------------------------------------------------------------------------
void AppMainWm5::OnTerminate()
{

    mScene = 0;
    mWireState = 0;

    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void AppMainWm5::OnIdle ()
{
    MeasureTime();
    PhysicsTick();
    GraphicsTick();
    UpdateFrameCount();
}
//----------------------------------------------------------------------------
bool AppMainWm5::OnKeyDown (unsigned char key, int x, int y)
{
    if (WindowApplication3::OnKeyDown(key, x, y))
    {
        return true;
    }

    switch (key)
    {
    case 'w':  // toggle wireframe
		printf("Toggle wireframe\n");
        mWireState->Enabled = !mWireState->Enabled;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void AppMainWm5::CreateScene ()
{

    // ** layout of scene graph **
    // scene
    //     room
    //         backwall
    //         floor
    //         sidewall1
    //         sidewall2
    //     balls

    mScene = new0 Node();
    mWireState = new0 WireState();
    mRenderer->SetOverrideWireState(mWireState);

    Node* room = new0 Node();
    mScene->AttachChild(room);
}

//----------------------------------------------------------------------------
void AppMainWm5::PhysicsTick ()
{
	// do some physics

    mScene->Update();

    mSimTime += mSimDeltaTime;
}
//----------------------------------------------------------------------------
void AppMainWm5::GraphicsTick ()
{
    mCuller.ComputeVisibleSet(mScene);

    if (mRenderer->PreDraw())
    {
        mRenderer->ClearBuffers();
        mRenderer->Draw(mCuller.GetVisibleSet());

        DrawFrameRate(8, GetHeight()-8, mTextColor);

        char message[256];
        sprintf(message, "Time = %5.2f", mSimTime);
        mRenderer->Draw(90, GetHeight()-8, mTextColor, message);

        mRenderer->PostDraw();
        mRenderer->DisplayColorBuffer();
    }
}

//----------------------------------------------------------------------------
