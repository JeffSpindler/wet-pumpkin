// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef IM_H
#define IM_H

#include <iostream>
#include <iomanip>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

namespace P3D
{

class Im
{
public:
    // This is based on an image object and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Im ()  : m_valid(false), m_width(640), m_height(480), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};
    Im (int width, int height) : m_valid(true), m_width(width), m_height(height), 
									m_tag(0), m_idx(0), m_time_usec(0), m_conf(0) {};

	virtual ~Im () {};

	
	bool m_valid;
	int m_tag;
	int m_idx;
	double m_time_usec;
	double m_conf;
	int m_width;
	int m_height;

	friend std::ostream& operator<<(std::ostream& os, const Im &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << pt.m_tag << std::setw(4) << pt.m_idx << 
				" [" << pt.m_width[0] << "x" << pt.m_height << "]" << std::endl; 
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
		ar & BOOST_SERIALIZATION_NVP( m_width );
		ar & BOOST_SERIALIZATION_NVP( m_height );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

BOOST_CLASS_VERSION(Im, 0)

typedef std::vector<Im> Im_v_t;
typedef std::deque<Im> Im_dq_t;
}

#endif
