// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/04)

#ifndef PT3D_H
#define PT3D_H

#include <iostream>
#include <iomanip>


#include "Wm5Vector3.h"

namespace P3D
{

class Pt3d : Wm5::Vector3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Pt3d ();  //
    ~Pt3d ();

    Pt3d (const Wm5::Vector3d& pos);

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
};

#include "Pt3d.inl"

typedef std::vector<Pt3d> Pt3d_v_t;
typedef std::deque<Pt3d> Pt3d_dq_t;

}

#endif
