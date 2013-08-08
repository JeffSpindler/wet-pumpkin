// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef PIXPT_H
#define PIXPT_H

#include <iostream>
#include <iomanip>


#include "Wm5Vector2.h"

namespace P3D
{

class PixPt : Wm5::Vector2f
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    PixPt ()  : m_valid(false), Wm5::Vector2f(Wm5::Vector3d::ZERO), m_tag(0), m_idx(0), 
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

	friend std::ostream& operator<<(std::ostream& os, const Pt3d &pt) {
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
};

typedef std::vector<PixPt> PixPt_v_t;
typedef std::deque<PixPt> PixPt_dq_t;

}

#endif
