// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/09/29)

#include "Scene.h"

#include "Wall.h"
#include "RigidBall.h"
#include "RigidBar.h"
#include "FrameGraphic.h"

const std::string Scene::default_filename = "save_default.txt";
//----------------------------------------------------------------------------
Scene::Scene (float radius)
    :
    mRadius(radius)
{

	m_frame3d = NULL;
	m_ball = NULL;
	m_bar = NULL;
    m_save_flag = false;
    m_save_filename = default_filename;

}
//
// Create the room and any objects and attach to scene graph
//
bool Scene::OnInitialize ()
{
    Node* room = new0 Node();
	AttachChild(room);

	CreateWalls();
	// create objects
	CreateFrame3d();
	//CreateBall();
	CreateBar();

	return(true);
}

//----------------------------------------------------------------------------

void Scene::OnTerminate()
{
	delete0( m_bar );
	delete0( m_ball );
	delete0( m_frame3d );
	delete0( m_wall[ROOM_BACK] );
	delete0( m_wall[ROOM_SIDE_L] );
	delete0( m_wall[ROOM_FLOOR] );
	delete0( m_wall[ROOM_SIDE_R] );

	m_bar = NULL;
	m_ball = NULL;
	m_frame3d = NULL;
}

// change scene depending on input
// clear out all values used
bool Scene::Change(Geom3d_dq_t &g3d_dq)
{
    if(g3d_dq.empty()) return(false);

    Vector3f pos((float) g3d_dq.back().m_pt[0], (float) g3d_dq.back().m_pt[1], (float) g3d_dq.back().m_pt[2]);

    m_bar->SetPosition(pos);

	// for bar  -- unit vector passed
    Vector3f unit_vec((float) g3d_dq.back().m_dir[1], (float) g3d_dq.back().m_dir[0], (float) g3d_dq.back().m_dir[2]);

    Quaternionf orient_q(unit_vec, (0.5f) * Mathf::PI);
    m_bar->SetQOrientation(orient_q);

    // archive as needed
    if(m_save_flag) {
       try {
        std::ofstream ofs;
        if(m_save_count)
	        ofs.open(m_save_filename, std::ios::app);
		else
	        ofs.open(m_save_filename, std::ios::trunc);

	    if(!ofs.good()) {
		    return(false);
	    }
        ofs << g3d_dq.back();
        ++m_save_count;
        // stop on too many records
        if(m_save_count > save_max) m_save_flag = false;
       }
       catch (const std::exception &ex) {
          std::cout << "archive " << m_save_filename << "  " << ex.what() << std::endl;
	    return(false);
       }        
	}

    g3d_dq.clear();
    return(true);
}

void Scene::Update()
{
	Node::Update();

	// update movable things
	if(m_ballNode != NULL) {
        m_ballNode->LocalTransform.SetTranslate(m_ball->GetPosition());
	}
	if(m_barNode != NULL) {
        m_barNode->LocalTransform.SetTranslate(m_bar->GetPosition());
        m_barNode->LocalTransform.SetRotate(m_bar->GetROrientation());
	}
}

void Scene::CreateWalls ()
{
    Float3 sideWallColor(170.0f/255.0f, 187.0f/255.0f, 219.0f/255.0f);
    Float3 backWallColor(209.0f/255.0f, 204.0f/255.0f, 180.0f/255.0f);
    Float3 floorColor(155.0f/255.0f, 177.0f/255.0f, 164.0f/255.0f);

	Vector3f back_pos(-290.0, 0.0, 120.0);
	Vector3f back_size(0.0, 300.0, 125.0);
	Vector3f sideL_pos(0.0, -250, 120.0);
	Vector3f sideL_size(300.0, 0.0, 125.0);
	Vector3f floor_pos(0.0, 0.0, -1.0);
	Vector3f floor_size(300.0, 300.0, 0.0);
	Vector3f sideR_pos(0.0, 250.0, 120.0);
	Vector3f sideR_size(300.0, 0.0, 125.0);

	m_wall[ROOM_BACK] = new0 Wall(this, back_pos, back_size, backWallColor);
	m_wall[ROOM_SIDE_L] = new0 Wall(this, sideL_pos, sideL_size, sideWallColor);
	m_wall[ROOM_FLOOR] = new0 Wall(this, floor_pos, floor_size, floorColor);
	m_wall[ROOM_SIDE_R] = new0 Wall(this, sideR_pos, sideR_size, sideWallColor);
}

void Scene::CreateBall ()
{
	float ball_radius = 4.0;
    float mass = 2.0f;

	m_ballNode = new0 Node();
	AttachChild(m_ballNode);

	m_ball = new0 RigidBall(m_ballNode, ball_radius);
	m_ball->SetMass(mass);
	m_ball->SetPosition(Vector3f(5.0f, 4.0f, 16.0f));
	m_ball->SetLinearMomentum(Vector3f(2.0f, 2.0f, -1.2f));
	m_ball->mForce = Force;
	m_ball->mTorque = Torque;
}

void Scene::CreateBar ()
{
	float bar_radius = 2.5;
    float mass = 10.0f;

	m_barNode = new0 Node();
	AttachChild(m_barNode);

	m_bar = new0 RigidBar(m_barNode, bar_radius);
	m_bar->SetMass(mass);
	m_bar->SetPosition(Vector3f(2.0f, -4.0f, 30.0f));
	m_bar->SetLinearMomentum(Vector3f(0.0f, 0.0f, 0.0f));
	Quaternionf orient_q(Vector3f::UNIT_X, Mathf::PI/2.0f);
	m_bar->SetQOrientation(orient_q);

	m_bar->mForce = Force;
	m_bar->mTorque = Torque;
}

void Scene::CreateFrame3d ()
{
	float frame_radius = 10.0;

	m_frame3dNode = new0 Node();
	AttachChild(m_frame3dNode);

	m_frame3d = new0 FrameGraphic(m_frame3dNode, frame_radius);
	m_frame3d->SetPosition(Vector3f(0.0f, 0.0f, 0.0));
	m_frame3d->SetMass(1.0f);
	m_frame3d->SetLinearMomentum(Vector3f(0.0f, 0.0f, 0.0f));
	m_frame3d->mForce = Force;
	m_frame3d->mTorque = Torque;
}

//----------------------------------------------------------------------------
Vector3f Scene::Force (float, float mass, const Vector3f&,
    const Quaternionf&, const Vector3f&, const Vector3f&, const Matrix3f&,
    const Vector3f&, const Vector3f&)
{
    const float gravityConstant = 9.81f;   // m/sec/sec
    const Vector3f gravityDirection = Vector3f(0.0f, 0.0f, -1.0f);
    return (mass*gravityConstant)*gravityDirection;
}
//----------------------------------------------------------------------------
Vector3f Scene::Torque (float, float, const Vector3f&,
    const Quaternionf&, const Vector3f&, const Vector3f&, const Matrix3f&,
    const Vector3f&, const Vector3f&)
{
    return Vector3f::ZERO;
}