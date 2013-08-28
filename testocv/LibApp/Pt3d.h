// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef PT3D_H
#define PT3D_H

#include <iostream>
#include <iomanip>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include "Wm5Vector3.h"

class Pt3d : Wm5::Vector3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Pt3d ()  : m_valid(false), Wm5::Vector3d(Wm5::Vector3d::ZERO), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};

	virtual ~Pt3d () {};

    Pt3d (const Wm5::Vector3d& pos) : m_valid(true), Wm5::Vector3d(pos), m_tag(0), m_idx(0), 
											m_time_usec(0), m_conf(0) {};
    Pt3d (double x, double y, double z) : m_valid(true), Wm5::Vector3d(x,y,z), m_tag(0), m_idx(0), 
											m_time_usec(0), m_conf(0) {};
	
	bool m_valid;
	int m_tag;
	int m_idx;
	double m_time_usec;
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
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_idx );
		ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

typedef std::vector<Pt3d> Pt3d_v_t;
typedef std::deque<Pt3d> Pt3d_dq_t;

BOOST_CLASS_VERSION(Pt3d, 0)

#endif
