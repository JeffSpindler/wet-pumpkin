//
// CamCal -- Camera Calibration Data
//
// Cam pose and intrinsic parms

// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef CAMCAL_H
#define CAMCAL_H

#include <iostream>
#include <iomanip>


#include "Wm5Vector3.h"
#incluce "Wm5Quaternion.h"

namespace P3D
{

class CamCal : Wm5::Vector3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    CamCal ()  : m_valid(false), Wm5::Vector3d(Wm5::Vector3d::ZERO), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};

	virtual ~CamCal () {};

    CamCal (const Wm5::Vector3d& pos) : m_valid(true), Wm5::Vector3d(pos), m_tag(0), m_idx(0), 
											m_time_usec(0), m_conf(0) {};
	
	bool m_valid;
	int m_cam;
    Wm5::Quaternion m_quat;
    double m_lens_correct[4];
    Wm5::vector3d u_vec;
    Wm5::vector3d v_vec;
    Wm5::vector3d a_vec;
    double m_intrinsic[4];
	double m_cam_pose_time_sec;
	double m_intrinsic_time_sec;
	double m_conf;

	friend std::ostream& operator<<(std::ostream& os, const Pt3d &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << pt.m_tag << std::setw(4) << pt.m_idx << 
				" (" << pt[0] << "," << pt[1] << "," << pt[2] << ")" << std::endl; 
		return os;
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_valid );
		ar & BOOST_SERIALIZATION_NVP( m_cam );
		//ar & BOOST_SERIALIZATION_NVP( m_idx );
		//ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

BOOST_CLASS_VERSION(CamCal, 0)

typedef std::vector<CamCal> camcal_v_t;
typedef std::deque<CamCal> camcal_dq_t;

}

#endif
