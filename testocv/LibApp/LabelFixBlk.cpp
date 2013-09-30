// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)



#include "LabelFixBlk.h"
#include "GlobalAccess.h"

#include "Pt3d.h"
#include "CamRay.h"

#include "Wm5Vector3.h"
#include "Wm5Ray3.h"
using namespace Wm5;

// label using fixtures
// Intersect rays for all fixtures, match fixtures created and label fixture pts
// there can be multiple fixtures, cands of each cam are grouped by tag

bool LabelFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;
	m_fix = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::FixStr));
	m_model = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::ModelStr));
	m_camrays = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CamRayStr));
	m_cur = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CurGeomStr));

	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));

	return (m_camrays != NULL && m_fix != NULL && m_model != NULL && m_cur != NULL);
};

bool LabelFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	if(m_camrays->Geoms().size() < 2) return(false);
	std::cout << m_name << " RUN camrays " << m_camrays->Geoms().size() << std::endl;

	intersectRays();

	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;

	return(true);
};

void LabelFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;
	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();

	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;
};

// labeling function

// interesect labeled rays and create cur ft3ds
// put all camrays into a multi-index set 
// intersect all rays with matching m_idx values

bool LabelFixBlk::intersectRays() 
{ 
	if(m_camrays->Geoms().size() < 2) return(false);
	std::cout << std::endl << " intersectRays " << m_camrays->Geoms().size() << std::endl;
	m_ray_set.clear();
	m_cur->Geoms().clear();

	CamRay camray_org(*(m_camrays->Geoms().begin()));	// use 1st camray to start
	//Vector3d unit_vec0(org.m_dir[0], org.m_dir[1], org.m_dir[2]);	// commpare this unit vec
	BOOST_FOREACH(Geom3d g3d, m_camrays->Geoms()) {
		m_ray_set.insert(g3d);
	}
	// now get pairs from different cams with same labels
	// as pairs are used delete from ray set and put in cur g3d set
	g3d_mi_idx_index_t::iterator ray_i, ray1st_i, ray2nd_i;
	int cur_idx;
	ray_i = m_ray_set.get<g3d_mi_idx>().begin();
	ray1st_i = ray_i;
	cur_idx = ray1st_i->m_idx;
	ray_i++;
	Pt3d cand_pt;	// candidate created
	while(ray_i != m_ray_set.get<g3d_mi_idx>().end()) {
		// check for next 
		if(ray_i->m_idx == cur_idx) {
			// intersect and add to cur g3d set
			cand_pt.findIntersect(CamRay(*ray_i), CamRay(*ray1st_i), 4.0);	// use max_int
			// save pt
			cand_pt.m_tag = m_cur->Geoms().size();
			m_cur->Geoms().push_back(cand_pt);
			// print
			std::cout << cand_pt << std::endl;			
			ray_i++;
			if(ray_i == m_ray_set.get<g3d_mi_idx>().end()) break; // done
		}
		// start new pair
		ray1st_i = ray_i;
		cur_idx = ray1st_i->m_idx;
		ray_i++;
	}

	// add some labeling functions - something to combine multi-cam cands.


	// something to label using fixture model


	if(Debug(3)) {
		std::cout << std::endl << "By Idx " << std::endl;
		print_g3d_by<g3d_mi_idx>(m_ray_set);
		std::cout << std::endl << "By Cam/Idx " << std::endl;
		print_g3d_by<g3d_mi_src_idx>(m_ray_set);
	}

	return(true);
};

//BOOST_CLASS_EXPORT(LabelFixBlk);
