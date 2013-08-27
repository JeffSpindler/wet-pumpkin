// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/05)
//
// Geometric 3d Object
//
#ifndef GEOM3D_H
#define GEOM3D_H

#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <iomanip>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

namespace P3D
{

class Geom3d
{
public:
    // This uses mulitple vectors to and a tag object with addl data required for use.
	//
    // Construction and destruction.
    Geom3d () : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0) {
		m_valid = false;
		m_type = INVALID;
		memset(m_pt, 0, sizeof(m_pt));
		memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
    Geom3d (int tag) : m_tag(tag), m_idx(0), m_time_usec(0), m_conf(0) {
		m_valid = true;
		m_type = POINT;
		memset(m_pt, 0, sizeof(m_pt));
		memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (int tag, double x, double y, double z) : m_tag(tag), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		m_pt[0] = x; m_pt[1] = y; m_pt[2] = z;
		memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (double x, double y, double z) : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		m_pt[0] = x; m_pt[1] = y; m_pt[2] = z;
		memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (double pos[]) : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		memcpy(m_pt, pos, sizeof(m_pt));
		memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}

	virtual ~Geom3d () {};

	bool m_valid;
	int m_type;		// pt/line/frame/moving pt/plane/moving frame/cam_ray/
	int m_tag;
	int m_idx;
	double m_time_usec;
	double m_conf;

	double m_pt[3];
	double m_dir[4];				// quaternion for rotation/frame slope for line normal for plane/spin
	double m_vel[3];				// 3 vector - speed is magnitude

	friend std::ostream& operator<<(std::ostream& os, const Geom3d &g3d) {
		if(!g3d.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << g3d.m_tag << std::setw(4) << g3d.m_idx << 
				" (" << g3d.m_pt[0] << "," << g3d.m_pt[1] << "," << g3d.m_pt[2] << ")" << std::endl; 
		return os;
	}
	enum {
		INVALID = 0,
		POINT = 1,
		LINE = 2,
		PLANE = 3,
		CAM_RAY = 4,
		FRAME = 10,
		SPIN = 11,
		PSEG = 12,
	};

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_valid );
		ar & BOOST_SERIALIZATION_NVP( m_type );
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_idx );
		ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );

		ar & BOOST_SERIALIZATION_NVP( m_pt );
		ar & BOOST_SERIALIZATION_NVP( m_dir );
		ar & BOOST_SERIALIZATION_NVP( m_vel );
	}
};

typedef std::vector<Geom3d> Geom3d_v_t;
typedef std::deque<Geom3d> Geom3d_dq_t;
typedef std::map<int, Geom3d> Geom3d_m_t;
}

BOOST_CLASS_VERSION(P3D::Geom3d, 0)

#endif
