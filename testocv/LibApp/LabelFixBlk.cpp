// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "LabelFixBlk.h"
#include "GlobalAccess.h"

#include "CamRay.h"
	
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
	if(m_camrays->Geoms().size() < 3) return(false);
	std::cout << m_name << " RUN camrays " << m_camrays->Geoms().size() << std::endl;

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

void LabelFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;
	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();

	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(LabelFixBlk);
