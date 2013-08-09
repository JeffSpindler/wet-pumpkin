// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef PIXPT_H
#define PIXPT_H

#include <iostream>
#include <iomanip>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include "Wm5Vector2.h"

namespace P3D
{

class PixPt : Wm5::Vector2f
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    PixPt ()  : m_valid(false), Wm5::Vector2f(Wm5::Vector2f::ZERO), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};
    PixPt (const Wm5::Vector2f& pos) : m_valid(true), Wm5::Vector2f(pos), m_tag(0), m_idx(0), 
											m_time_usec(0), m_conf(0) {};

	virtual ~PixPt () {};

	
	bool m_valid;
	int m_tag;
	int m_idx;
	double m_time_usec;
	double m_conf;
	int m_area;

	friend std::ostream& operator<<(std::ostream& os, const PixPt &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << pt.m_tag << std::setw(4) << pt.m_idx << 
				" (" << pt[0] << "," << pt[1] << ")" << std::endl; 
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

typedef std::vector<PixPt> PixPt_v_t;
typedef std::deque<PixPt> PixPt_dq_t;
}

BOOST_CLASS_VERSION(P3D::PixPt, 0)

#endif
