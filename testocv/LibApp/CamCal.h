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

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include "Wm5Vector3.h"
#include "Wm5Quaternion.h"

namespace P3D
{

class CamCal : Wm5::Vector3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    CamCal ()  : m_valid(false), Wm5::Vector3d(Wm5::Vector3d::ZERO), m_cam(0), 
						m_quat(Wm5::Quaterniond::IDENTITY), m_conf(0) {
		memset(m_lens_correct, 0, sizeof(m_lens_correct));
		u_vec = Wm5::Vector3d::ZERO;
		v_vec = Wm5::Vector3d::ZERO;
		a_vec = Wm5::Vector3d::ZERO;
		memset(m_intrinsic, 0, sizeof(m_intrinsic));
		m_cam_pose_time_sec = 0; m_intrinsic_time_sec = 0;
	};

	virtual ~CamCal () {};

    CamCal (int cam) : m_valid(true), Wm5::Vector3d(Wm5::Vector3d::ZERO), m_cam(cam), 
							m_quat(Wm5::Quaterniond::IDENTITY), m_conf(0)
	{
		memset(m_lens_correct, 0, sizeof(m_lens_correct));
		u_vec = Wm5::Vector3d::ZERO;
		v_vec = Wm5::Vector3d::ZERO;
		a_vec = Wm5::Vector3d::ZERO;
		memset(m_intrinsic, 0, sizeof(m_intrinsic));
		m_cam_pose_time_sec = 0; m_intrinsic_time_sec = 0;
	};

public:	
	bool m_valid;
	int m_cam;
    Wm5::Quaterniond m_quat;
    double m_lens_correct[4];
    Wm5::Vector3d u_vec;
    Wm5::Vector3d v_vec;
    Wm5::Vector3d a_vec;
    double m_intrinsic[4];
	double m_cam_pose_time_sec;
	double m_intrinsic_time_sec;
	double m_conf;

	friend std::ostream& operator<<(std::ostream& os, const CamCal &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << pt.m_cam << 
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
		//ar & BOOST_SERIALIZATION_NVP( m_idx );		//ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

typedef std::vector<CamCal> camcal_v_t;
typedef std::deque<CamCal> camcal_dq_t;
}

BOOST_CLASS_VERSION(P3D::CamCal, 0)

#endif
