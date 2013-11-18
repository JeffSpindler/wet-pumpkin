// Perform3-D LLC
// Copyright (c) 2013
//
//
// File Version: 1.0.0 (2013/08/26)

#include "AppMainWm5.h"

#include "CrunchApp.h"
#include "BlockExports.h"

#include "CommGeom3dClient.h"
#include "CommGeom3dServer.h"

#include "RigidBall.h"
#include "FrameGraphic.h"

WM5_WINDOW_APPLICATION(AppMainWm5);

//----------------------------------------------------------------------------
AppMainWm5::AppMainWm5 ()
    : m_name("AppMainWm5"),
    WindowApplication3("AppMainWm5", 0, 0, 640, 480,
        Float4(1.0f, 1.0f, 1.0f, 1.0f)),
        mTextColor(0.0f, 0.0f, 0.0f, 1.0f)
{
    mSimTime = 0.0f;
    mSimDeltaTime = 1.0f/10.0f;
	m_app = NULL;
	m_comm_track_client = NULL;
	m_comm_ray_client = NULL;
	m_comm_server = NULL;
    mScene = NULL;
    mWireState = NULL;
    mCamera = NULL;
}
//----------------------------------------------------------------------------
bool AppMainWm5::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }
    std::cout << "Copyright 2013 Perform3-D LLC" << std::endl << "All rights reserved" << std::endl;
    std::cout << std::endl << "Bar Tracking Display App" << std::endl << std::endl;

	//m_comm_server = new CommGeom3dServer;
    //m_comm_server->StartServer(13110);

	m_comm_track_client = new CommGeom3dClient;
	std::string filename = "CommTrackClient.xml";
	if(!m_comm_track_client->load(filename)) {
		m_comm_track_client->save(filename);
	} else {
		std::cout << "loaded " << filename << "  client setup\n\n";
	}
	m_comm_track_client->StartClient();	// startup everything

	m_comm_ray_client = new CommGeom3dClient;
	filename = "CommRayClient.xml";
	if(!m_comm_ray_client->load(filename)) {
		m_comm_ray_client->save(filename);
	} else {
		std::cout << "loaded " << filename << "  client setup\n\n";
	}
	m_comm_ray_client->StartClient();	// startup everything


	std::cout << m_name << "  OnInitialize\n";
	// Init app
	m_app = new CrunchApp(m_name);
	std::cout << *m_app << std::endl << std::endl;

	m_app->onInitialize();

	Im im_vga(640, 480);
	std::cout << im_vga << std::endl << std::endl;

	Pt3d pt(1,2,3);
	std::cout << pt << std::endl << std::endl;

	// Init WM5 Window
    // Set up the camera.
    mCamera->SetFrustum(60.0f, GetAspectRatio(), 1.0f, 1000.0f);
    float angle = 0.1f*Mathf::PI;
    float cs = Mathf::Cos(angle), sn = Mathf::Sin(angle);
    APoint camPosition(270.0f, 0.0f, 120.0f);
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

    InitializeCameraMotion(0.01f, 0.001f);
    InitializeObjectMotion(mScene);

    return true;
}
//----------------------------------------------------------------------------
void AppMainWm5::OnTerminate()
{

    mScene = 0;
    mWireState = 0;

	if(m_app != NULL) { 
		m_app->OnTerminate();
		delete m_app;
	}
	if(m_comm_track_client != NULL) {
		m_comm_track_client->Terminate();
		delete m_comm_track_client;
	}
	if(m_comm_ray_client != NULL) {
		m_comm_ray_client->Terminate();
		delete m_comm_ray_client;
	}
	if(m_comm_server != NULL) {
		m_comm_server->Terminate();
		delete m_comm_server;
	}

    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void AppMainWm5::OnIdle ()
{
    MeasureTime();
    PhysicsTick();
    GraphicsTick();
    UpdateFrameCount();

	// check for comms
	if(m_comm_ray_client->Update(m_comm_g3d_dq)) {
		// load up the g3ds
		//BOOST_FOREACH(Geom3d pt, m_comm_g3d_dq) {
			// send app geom3d
		m_app->addInput(m_comm_g3d_dq);
		//}
		m_comm_g3d_dq.clear();
	}
	if(m_comm_track_client->Update(m_comm_g3d_dq)) {
        std::cout << m_comm_g3d_dq.back() << std::endl; 
        mScene->Change(m_comm_g3d_dq);
        // keep from growing forever
        while(m_comm_g3d_dq.size() > 500) {
            m_comm_g3d_dq.pop_front();
		}
	}

}
//----------------------------------------------------------------------------
bool AppMainWm5::OnKeyDown (unsigned char key, int x, int y)
{
    if (WindowApplication3::OnKeyDown(key, x, y))
    {
        return true;
    }
	//boost::to_upper(key);
    switch (key)
    {
    case 'w':  // toggle wireframe
		printf("Toggle wireframe\n");
        mWireState->Enabled = !mWireState->Enabled;
        return true;
	case 'q':  { // test data inserted
		static int test_id = 0;
		static Geom3d g3d(0);
		static Geom3d_v_t model_v;
		static Geom3d_dq_t g3d_dq;

		printf("Add Test Data %d\n", ++test_id);
		m_app->testModel(test_id, model_v);
		m_app->testCamRay(test_id, model_v, g3d_dq);
		m_app->addInput(g3d_dq);
		return true;
		}
	case 's': { // toggle save mode
        mScene->setSave(!mScene->getSave());
        return true;
	    }
	case 'u': { // make test seq
		printf("Make Test Seq\n");
		m_app->testPixSeq();
		m_app->testTrajSeq();
		return true;
		}
	case '/':
	case '?':
		print_help();
		return true;
	}

    return false;
}
void AppMainWm5::print_help()
{
	printf("\n\nvalid keys\n\n");
	printf("Q - test app\n");
	printf("U - Write Seq files\n");
	printf("W - toggle wireframe\n");
	printf("? - list help\n");
}

//
void AppMainWm5::CreateScene ()
{

    // ** layout of scene graph **
    // scene
    //     room
    //         backwall
    //         floor
    //         sidewall1
    //         sidewall2

    mScene = new0 Scene();
    mWireState = new0 WireState();
    mRenderer->SetOverrideWireState(mWireState);

	mScene->OnInitialize();
}

//----------------------------------------------------------------------------
void AppMainWm5::PhysicsTick ()
{
	// do some physics
	// handle changes to real-world via comms

	// handle collisions of sim given current positions and velocities

	// update the objects for graphics display
    mScene->Update();

    mSimTime += mSimDeltaTime;
}
//----------------------------------------------------------------------------
void AppMainWm5::GraphicsTick ()
{

    if (MoveCamera())
    {
        //mCuller.ComputeVisibleSet(mScene);
    }

    if (MoveObject())
    {
        mScene->Update();
        //mCuller.ComputeVisibleSet(mScene);
    }
    
	//mScene->Update();
    mCuller.ComputeVisibleSet(mScene);

    if (mRenderer->PreDraw())
    {
        mRenderer->ClearBuffers();
        mRenderer->Draw(mCuller.GetVisibleSet());

        DrawFrameRate(8, GetHeight()-8, mTextColor);

        char message[256];
        sprintf(message, "Time = %5.2f", mSimTime);
        mRenderer->Draw(90, GetHeight()-8, mTextColor, message);

        if(mScene->getSave()) {
            char save_message[] = "SAVE ON";
            mRenderer->Draw(300, GetHeight()-8, mTextColor, save_message);
		}

        mRenderer->PostDraw();
        mRenderer->DisplayColorBuffer();
    }
}

//----------------------------------------------------------------------------
