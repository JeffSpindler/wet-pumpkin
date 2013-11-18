// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/04)

#ifndef CAMRAY_H
#define CAMRAY_H

#include "Global.h"
#include "Geom3d.h"

#include "Wm5Vector3.h"
#include "Wm5Ray3.h"

class CamRay : public Geom3d
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    CamRay ()  : m_vec(Wm5::Vector3d::ZERO), m_unit_vec(Wm5::Vector3d::UNIT_Z) {};
    CamRay (const Wm5::Vector3d& pos) :  Geom3d(pos[0],pos[1],pos[2]), m_vec(pos) {};
    CamRay (double x, double y, double z) : Geom3d(x,y,z), m_vec(x,y,z) {};
    CamRay (double pos[3]) : Geom3d(pos), m_vec(pos[0],pos[1],pos[2]) {};
	CamRay(const Geom3d &g3d) : Geom3d(g3d), m_vec(g3d.m_pt[0],g3d.m_pt[1],g3d.m_pt[2]), 
						m_unit_vec(g3d.m_dir[0],g3d.m_dir[1],g3d.m_dir[2]) { 
			if(g3d.m_type != CAM_RAY) m_valid = false; m_unit_vec.Normalize(); };

	virtual ~CamRay () {};

	Wm5::Vector3d m_vec;
	Wm5::Vector3d m_unit_vec;

	double findAngle(CamRay &camray) { // find angle to other camray
		double angle = acos(m_unit_vec.Dot(camray.m_unit_vec));
		return(angle);
	};
	double findCrossAngle(CamRay &camray1, CamRay &camray2) { // find angle to cross of 1st and 2nd camray
		double angle = acos(m_unit_vec.Dot(camray1.m_unit_vec.UnitCross(camray2.m_unit_vec)));
		return(angle);
	};

	friend std::ostream& operator<<(std::ostream& os, const CamRay &pt) {
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

typedef std::vector<CamRay> CamRay_v_t;
typedef std::deque<CamRay> CamRay_dq_t;

BOOST_CLASS_VERSION(CamRay, 0)

#endif
