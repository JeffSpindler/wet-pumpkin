// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "TrackFixBlk.h"
#include "GlobalAccess.h"
	
// Given fixtures found, track them and find the current frames for each

bool TrackFixBlk::doSetup(IGlobalAccess *data_access) 
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

bool TrackFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << " RUN size " << std::endl;
	//BOOST_FOREACH(PixPt pt, m_pts->PixPts()) {
	//	// put in geom3d out
	//	Geom3d g3d(pt.m_tag);
	//	m_geoms->Geoms().push_back(g3d);
	//}
	//m_pts->PixPts().clear();
	return(true);
};

void TrackFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;

	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();

	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(TrackFixBlk);
