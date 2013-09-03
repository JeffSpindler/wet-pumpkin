// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "GlobalAccess.h"
#include "LoadFixBlk.h"
	
// Test class for logic block

bool LoadFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;

	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));
	m_geoms = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::PixAppQuStr));
	return (m_cals != NULL && m_pts != NULL && m_geoms != NULL);
};

bool LoadFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << " NULL runtime " << std::endl;
	return(true);
};

void LoadFixBlk::doPrint() 
{ 
	std::cout << m_name << "  " << m_str_cals << " " << m_str_pts << std::endl;
	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();
	if(m_geoms != NULL) m_geoms->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(LoadFixBlk);
