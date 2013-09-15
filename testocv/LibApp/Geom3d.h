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
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>

class Geom3d
{
public:
    // This uses mulitple vectors to and a tag object with addl data required for use.
	//
    // Construction and destruction.
    Geom3d () : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0) {
		m_valid = false;
		m_type = INVALID;
		m_src = 0;
		memset(m_pt, 0, sizeof(m_pt));
		//memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
    Geom3d (int tag) : m_tag(tag), m_idx(0), m_time_usec(0), m_conf(0) {
		m_valid = true;
		m_type = POINT;
		m_src = 0;
		memset(m_pt, 0, sizeof(m_pt));
		//memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (int tag, double x, double y, double z) : m_tag(tag), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		m_src = 0;
		m_pt[0] = x; m_pt[1] = y; m_pt[2] = z;
		//memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (double x, double y, double z) : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		m_src = 0;
		m_pt[0] = x; m_pt[1] = y; m_pt[2] = z;
		//memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}
	Geom3d (double pos[]) : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true) {
		m_type = POINT;
		m_src = 0;
		memcpy(m_pt, pos, sizeof(m_pt));
		//memset(m_vel, 0, sizeof(m_vel));
		memset(m_dir, 0, sizeof(m_dir));
	}

	virtual ~Geom3d () {};

	bool m_valid;
	int m_type;		// pt/line/frame/moving pt/plane/moving frame/cam_ray/
	int m_tag;		// frame num
	int m_idx;		// index
	int m_src;		// cam num/cam mask source of data
	double m_time_usec;		// timestamp
	double m_conf;	// confidence/quality percent (0-1.0)

	double m_pt[3];
	double m_dir[4];				// quaternion for rotation/frame slope for line normal for plane/spin
	//double m_vel[3];				// 3 vector - speed is magnitude

	friend std::ostream& operator<<(std::ostream& os, const Geom3d &g3d) {
		if(!g3d.m_valid) {
			os << " invalid\n";
			return os;
		}
		switch(g3d.m_type) {
			case(POINT):
				os << "PT  "; 
				break;
			case(LINE):
				os << "LN  ";
				break;
			case(PLANE):
				os << "PL  ";
				break;	
			case(CAM_RAY):
				os << "CR  ";
				break;
			case(FRAME):
				os << "FRM ";
				break;
			case(SPIN):
				os << "SP  ";
				break;
			case(VEL):
				os << "VEL ";				
				break;
			default:
				os << "UNKN";
				break;
		}

		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << g3d.m_tag << std::setw(4) << g3d.m_idx << std::setw(4) << g3d.m_src
				<< " (" << g3d.m_pt[0] << "," << g3d.m_pt[1] << "," << g3d.m_pt[2] << ")"; 
		switch(g3d.m_type) {
			case(CAM_RAY):
			case(PLANE):
			case(LINE):
				os.precision(3);
				os << " (" << g3d.m_dir[0] << "," << g3d.m_dir[1] << "," << g3d.m_dir[2] << ")" << std::endl; 
				break;
			case(FRAME):
			case(SPIN):
				os.precision(3);
				os << " [" << g3d.m_dir[0] << "," << g3d.m_dir[1] << "," << g3d.m_dir[2] << "," << g3d.m_dir[3] << "]" << std::endl; 
				break;
			case(VEL):
				os.precision(2);
				os << " (" << g3d.m_dir[0] << "," << g3d.m_dir[1] << "," << g3d.m_dir[2] << ")" << std::endl; 
				break;
			case(POINT):
				os << std::endl;
			default:
				break;
		}
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
        VEL = 13,
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
		ar & BOOST_SERIALIZATION_NVP( m_src );
		ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );

		ar & BOOST_SERIALIZATION_NVP( m_pt );
		ar & BOOST_SERIALIZATION_NVP( m_dir );
		//ar & BOOST_SERIALIZATION_NVP( m_vel );
	}
};

typedef std::vector<Geom3d> Geom3d_v_t;
typedef std::deque<Geom3d> Geom3d_dq_t;
typedef std::map<int, Geom3d> Geom3d_m_t;

BOOST_CLASS_VERSION(Geom3d, 0)

#endif
