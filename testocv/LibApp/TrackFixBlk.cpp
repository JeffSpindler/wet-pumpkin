// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "TrackFixBlk.h"
#include "GlobalAccess.h"
	
// Load fixtures at setup, null at run-time

bool TrackFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;

	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));
	m_geoms = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::PixAppQuStr));
	return (m_cals != NULL && m_pts != NULL && m_geoms != NULL);
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
	std::cout << m_name << "  " << m_str_cals << " " << m_str_pts << std::endl;
	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();
	if(m_geoms != NULL) m_geoms->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(TrackFixBlk);
