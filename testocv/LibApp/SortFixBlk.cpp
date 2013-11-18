// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "SortFixBlk.h"
#include "GlobalAccess.h"

#include "CamRay.h"
	
// review the camrays and partition according to min dist between fts
// use the angle between rays to decide who is close
// partition the elements by relative angles.
// evaluate all rays and only pass potentially valid labelings. use m_tag for fix tag chosen
// m_idx is identifier of set of compatible rays in order with conf set.
//
// If there are exactly two rays found to match some pair of the fixture then the order of the labeling is arbitrary.
// If three or more rays found then if conf 100 they match 3d angles within error margin.

const std::string SortFixBlk::block_name = "SortFixBlk";

bool SortFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;
	m_fix = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::FixStr));
	m_model = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::ModelStr));
	m_camrays = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CamRayStr));
	m_cur = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CurGeomStr));

	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));
	
	if(m_cals != NULL) m_num_cams = m_cals->m_num_cams; else m_num_cams = 0;

	return (m_camrays != NULL && m_fix != NULL && m_model != NULL && m_cur != NULL);
};

bool SortFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << " RUN size " << m_camrays->Geoms().size() << std::endl;
	if(m_camrays->Geoms().size() < 3) return(false);

	// label the cam ray cands
	// tags should match fix pts
	// idx should be in order of best 0 to worst 1
	// each cam ray cand will have conf setup
	//
	static CamRay_v_t cam_ray_v;
	for(int c=0;c<m_num_cams;c++) {
		cam_ray_v.clear();
		BOOST_FOREACH(Geom3d g3d, m_camrays->Geoms()) {
			if(g3d.m_src != c) continue;
			CamRay camray(g3d);
			camray.m_tag = cam_ray_v.size();
			cam_ray_v.push_back(camray);
		}
		// make the pairs to work with
		createRayPairs(cam_ray_v);
		findBestRays(cam_ray_v);	// setup m_cur with best
	}

	return(true);
};

void SortFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;
	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();
	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;
};

// for each pair of cam rays, find the angle between them
bool SortFixBlk::createRayPairs(CamRay_v_t &cam_ray_v)
{
	int count = 0;

	if(cam_ray_v.empty()) return(false);
	std::cout << "createRayPairs cam " << cam_ray_v.front().m_src << " size " << cam_ray_v.size() << std::endl;

	m_ray_pair_set.clear();
	CamRay camray_front(cam_ray_v.front());	// use front for 1st
	CamRay camray_back(cam_ray_v.back());	// use back for 2nd

	int cam = cam_ray_v.front().m_src;
	//Vector3d unit_vec0(org.m_dir[0], org.m_dir[1], org.m_dir[2]);	// commpare this unit vec
	for(unsigned i=0;i<cam_ray_v.size();i++) {
		if(cam_ray_v[i].m_src != cam) continue;	// only do 1st cam for now
		CamRay camray_i(cam_ray_v[i]);

			for(unsigned j=i+1;j<cam_ray_v.size();j++) {
				if(cam_ray_v[j].m_src != cam) continue;	// only do 1st cam for now
				CamRay camray_j(cam_ray_v[j]);
				float angle = (float) camray_j.findAngle(camray_i);
				float norm_angle = 0; //(float) camray_j.findCrossAngle(camray_front, camray_back);
				
				PairSort2 pair(count, camray_i.m_tag, camray_j.m_tag, angle, norm_angle, 0);
				m_ray_pair_set.insert(pair);

				std::cout << pair;
				count++;
			}
	}
	std::cout << "final pairs by tag " << std::endl;
	print_ps2_by<ps2_mi_tag>(m_ray_pair_set);

	return(!m_ray_pair_set.empty());
}

// check for match with fixture rays
bool SortFixBlk::findBestRays(CamRay_v_t &cam_ray_v)
{

	return(true);
}

//BOOST_CLASS_EXPORT(SortFixBlk);
