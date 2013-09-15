// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/15)

// create and compute with points
//

#ifndef PT3D_H
#define PT3D_H

#include "Global.h"
#include "Geom3d.h"
#include "CamRay.h"

#include "Wm5Vector3.h"
#include "Wm5Ray3.h"
#include "Wm5DistRay3Ray3.h"

class Pt3d : public Geom3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Pt3d ()  : m_pt(Wm5::Vector3d::ZERO) {};
    Pt3d (const Wm5::Vector3d& pos) :  Geom3d(pos[0],pos[1],pos[2]), m_pt(pos) {};
    Pt3d (double x, double y, double z) : Geom3d(x,y,z), m_pt(x,y,z) {};
    Pt3d (double pos[3]) : Geom3d(pos), m_pt(pos[0],pos[1],pos[2]) {};
	Pt3d(Geom3d &g3d) : Geom3d(g3d), m_pt(g3d.m_pt[0],g3d.m_pt[1],g3d.m_pt[2]) { 			
			if(g3d.m_type != POINT) m_valid = false; };

	virtual ~Pt3d () {};

	Wm5::Vector3d m_pt;
	double m_int_err;	// int dist

	// find int and setup this Pt3d
	// leaves m_int_err set and finds m_conf
	bool findIntersect(CamRay &r1, CamRay &r2, double max_int_err = 4.0) { // find int
		Wm5::DistRay3Ray3d ray_ray_int(Wm5::Ray3d(r1.m_pt, r1.m_unit_vec), Wm5::Ray3d(r2.m_pt, r2.m_unit_vec));
		// invalid
		m_valid = false;
		m_type = INVALID;
		m_time_usec = r1.m_time_usec;
		m_conf = 0.0;
		if(r1.m_src == r2.m_src) {
			return(false);
		}

		m_int_err = ray_ray_int.Get();
		double ray1_parm = ray_ray_int.GetRay0Parameter();
		double ray2_parm = ray_ray_int.GetRay1Parameter();

		Wm5::Vector3d pt1 = r1.m_pt + ray1_parm * r1.m_unit_vec;
		Wm5::Vector3d pt2 = r2.m_pt + ray2_parm * r2.m_unit_vec;

		m_pt = (pt1 + pt2) * 0.5; // use avg pt

		if(m_int_err < max_int_err) {
			m_valid = true;
			m_type = POINT;
			m_src = 3; // set cam mask
			m_idx = r1.m_idx; // use 1st ray idx
			m_time_usec = r1.m_time_usec;
			m_conf = ( (max_int_err - m_int_err) / max_int_err );
		}
		return(m_valid);
	};

	friend std::ostream& operator<<(std::ostream& os, const Pt3d &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << pt.m_tag << std::setw(4) << pt.m_idx << 
				" (" << pt.m_pt[0] << "," << pt.m_pt[1] << "," << pt.m_pt[2] << ")" << std::endl; 
		return os;
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Geom3d);
	}
};

typedef std::vector<CamRay> Pt3d_v_t;
typedef std::deque<CamRay> Pt3d_dq_t;

BOOST_CLASS_VERSION(Pt3d, 0)

#endif
