// Perform3-D LLC
// Copyright (c) 2013
// Show a 3d scene
//
// File Version: 1.0.0 (2013/09/29)

#ifndef SCENE_H
#define SCENE_H

#include "Wm5Node.h"
#include "Wm5Vector3.h"
#include "Wm5Matrix3.h"
#include "Wm5Quaternion.h"

#include "Global.h"
#include "Geom3d.h"

using namespace Wm5;

class FrameGraphic;
class RigidBall;
class RigidBar;
class Wall;

class Scene : public Node
{
public:
    Scene (float radius = 100.0f);
    virtual bool OnInitialize ();
    virtual void OnTerminate ();

    bool Change(Geom3d_dq_t &g3d_dq);
	void Update();

	void CreateWalls();
	void CreateBall ();
	void CreateBar ();
	void CreateFrame3d ();

	float GetRadius () const { return mRadius; };
	bool setSave(bool save_mode) 
	{ 
		m_save_flag = save_mode;
        m_save_count = 0;
	    return m_save_flag; 
	};
	bool getSave() { return m_save_flag; };

private:
    static const int save_max = 35000;  // max recs to save

    bool m_save_flag;       // save data to file
    int m_save_count;       // count saved records
    std::string m_save_filename;    // filename to save data to
    float mRadius;

	// nodes
	NodePtr m_ballNode;
	NodePtr m_barNode;
	NodePtr m_frame3dNode;

	// graphic elements
	FrameGraphic *m_frame3d;
	RigidBall *m_ball;
	RigidBar *m_bar;
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
    static const std::string default_filename;
	enum {
		ROOM_BACK,		// -X
		ROOM_SIDE_L,	// +Y
		ROOM_FLOOR,		// -Z
		ROOM_SIDE_R,	// -Y
		ROOM_TOP,		// +Z
		ROOM_FRONT,		// +X
	};

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(mRadius);
	}
};

typedef Pointer0<Scene> ScenePtr;

BOOST_CLASS_VERSION(Scene, 1)

#endif
