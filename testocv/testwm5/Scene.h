// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/29)

#ifndef SCENE_H
#define SCENE_H

#include "Wm5Node.h"
#include "Wm5Vector3.h"
#include "Wm5Matrix3.h"
#include "Wm5Quaternion.h"

using namespace Wm5;

class FrameGraphic;
class RigidBall;
class Wall;

class Scene : public Node
{
public:
    Scene (float radius = 100.0f);
    virtual bool OnInitialize ();
    virtual void OnTerminate ();

	void Update();

	void CreateWalls();
	void CreateBalls ();
	void CreateFrame3d ();

	float GetRadius () const { return mRadius; };


private:
    float mRadius;

	// nodes
	NodePtr m_ballNode;
	NodePtr m_frame3dNode;


	// graphic elements
	FrameGraphic *m_frame3d;
	RigidBall *m_ball;
	Wall *(m_wall)[6];

    // External forces and torques for objects.
    static Vector3f Force (float time, float mass, const Vector3f& position,
        const Quaternionf& quatOrient, const Vector3f& linearMomentum,
        const Vector3f& angularMomentum, const Matrix3f& rotOrient,
        const Vector3f& linearVelocity, const Vector3f& angularVelocity);

    static Vector3f Torque (float time, float mass, const Vector3f& position,
        const Quaternionf& quatOrient, const Vector3f& linearMomentum,
        const Vector3f& angularMomentum, const Matrix3f& rotOrient,
        const Vector3f& linearVelocity, const Vector3f& angularVelocity);

public:
	enum {
		ROOM_BACK,		// -X
		ROOM_SIDE_L,	// +Y
		ROOM_FLOOR,		// -Z
		ROOM_SIDE_R,	// -Y
		ROOM_TOP,		// +Z
		ROOM_FRONT,		// +X
	};
};

typedef Pointer0<Scene> ScenePtr;

#endif
