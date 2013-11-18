// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/04)

#ifndef PAIRSORT2_H
#define PAIRSORT2_H

#include "Global.h"

// use this to make pairs of rays for sorting/labeling
//
// 

class PairSort2
{
public:
    // This is just some elements to sort and search with
	//

    // Construction and destruction.
    PairSort2 ()  : m_tag(0), m_idx(0), m_src1(0), m_src2(0), m_dist(0), m_dist2(0), m_conf(0) {};
    PairSort2 (int tag, int src1, int src2, float ang_dist, float norm_ang_dist, float conf) :  
					m_tag(tag), m_idx(0), m_src1(src1), m_src2(src2), m_dist(ang_dist),
					m_dist2(norm_ang_dist), m_conf(conf) {};

	friend std::ostream& operator<<(std::ostream& os, const PairSort2 &ps) {
		os << boost::format("%4d %4d [%4d,%4d] ") % ps.m_tag % ps.m_idx % ps.m_src1 % ps.m_src2;
		os << boost::format(" %6.3f %6.3f  %4.2f)\n") % ps.m_dist % ps.m_dist2 % ps.m_conf;
		return os;
	}
public:
	int m_tag;		// tag
	int m_idx;		// index
	int m_src1;		// one side
	int m_src2;		// other side
	float m_dist;	// 1st dist
	float m_dist2;	// 2nd dist
	float m_conf;	// conf percentage 0-1.0

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_idx );
		ar & BOOST_SERIALIZATION_NVP( m_src1 );
		ar & BOOST_SERIALIZATION_NVP( m_src2 );
		ar & BOOST_SERIALIZATION_NVP( m_dist );
		ar & BOOST_SERIALIZATION_NVP( m_dist2 );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

typedef std::vector<PairSort2> PairSort2_v_t;
typedef std::deque<PairSort2> PairSort2_dq_t;

BOOST_CLASS_VERSION(PairSort2, 0)





#endif
