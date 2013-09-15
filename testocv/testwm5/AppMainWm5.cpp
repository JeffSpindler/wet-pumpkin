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
	m_comm_client = NULL;
	m_comm_server = NULL;
}
//----------------------------------------------------------------------------
bool AppMainWm5::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }

	m_comm_server = new CommGeom3dServer;
    m_comm_server->StartServer(13110);

	//m_comm_client = new CommGeom3dClient;
    //m_comm_client->StartClient(std::string("127.0.0.1"), std::string("13110"));

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

	if(m_app != NULL) { 
		m_app->OnTerminate();
		delete m_app;
	}
	if(m_comm_client != NULL) {
		m_comm_client->Terminate();
		delete m_comm_client;
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
	if(m_comm_server->Update(m_comm_g3d_dq)) {
		// load up the g3ds
		//BOOST_FOREACH(Geom3d pt, m_comm_g3d_dq) {
			// send app geom3d
		m_app->addInput(m_comm_g3d_dq);
		//}
		m_comm_g3d_dq.clear();
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
		static Geom3d_dq_t g3d_dq;

		printf("Add Test Data %d\n", ++test_id);
		m_app->testCamRay(test_id, g3d_dq);
		m_app->addInput(g3d_dq);
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
