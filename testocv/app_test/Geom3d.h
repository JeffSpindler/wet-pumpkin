// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/05)

#ifndef GEOM3D_H
#define GEOM3D_H

#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <iomanip>

namespace P3D
{

class Geom3d
{
public:
    // This uses mulitple vectors to and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Geom3d ();  //
    ~Geom3d ();

    Geom3d (double pos[3]);

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
		POINT = 0,
		LINE,
		PLANE,
		CAM_RAY,
		FRAME,
		SPIN,
	};
};

#include "Geom3d.inl"

typedef std::vector<Geom3d> Geom3d_v_t;
typedef std::deque<Geom3d> Geom3d_dq_t;
typedef std::map<int, Geom3d> Geom3d_m_t;
}

#endif
