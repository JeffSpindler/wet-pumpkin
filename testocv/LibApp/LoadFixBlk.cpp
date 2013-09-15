// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "GlobalAccess.h"
#include "LoadFixBlk.h"
	
// Test class for logic block

bool LoadFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;

	// may need to reload fix or model
	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));
	m_fix = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::FixStr));
	m_model = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::ModelStr));
	m_camrays = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CamRayStr));
	if(Debug(1)) {
		doPrint();	// show what's available
	}

	return (m_cals != NULL && m_pts != NULL && m_camrays != NULL && m_fix != NULL && m_model != NULL);
};

bool LoadFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	if(Debug(3)) {
		doPrint();	// show what's in pipeline
	}

	return(true);
};

void LoadFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;
	if(m_camrays != NULL) m_camrays->print();
	if(m_fix != NULL) m_fix->print();
	if(m_model != NULL) m_model->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(LoadFixBlk);
