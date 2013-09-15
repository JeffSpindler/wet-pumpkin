// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "SortFixBlk.h"
#include "GlobalAccess.h"

#include "CamRay.h"
	
// review the camrays and partition according to min dist between fts
// use the angle between rays to decide who is close
// partition the elements by relative angles.
//

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

	return (m_camrays != NULL && m_fix != NULL && m_model != NULL && m_cur != NULL);
};

bool SortFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << " RUN size " << m_camrays->Geoms().size() << std::endl;
	if(m_camrays->Geoms().size() < 3) return(false);

	CamRay camray_org(*(m_camrays->Geoms().begin()));	// use 1st camray to start
	//Vector3d unit_vec0(org.m_dir[0], org.m_dir[1], org.m_dir[2]);	// commpare this unit vec
	BOOST_FOREACH(Geom3d g3d, m_camrays->Geoms()) {
		CamRay camray(g3d);
		double angle = camray.findAngle(camray_org);
		std::cout << "angle " << angle << " " << g3d;
		//m_camrays->Geoms().push_back(g3d);
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

//BOOST_CLASS_EXPORT(SortFixBlk);
