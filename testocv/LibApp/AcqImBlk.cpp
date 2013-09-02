// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "AcqImBlk.h"

bool AcqImBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;
	m_val1 = 0;

	m_ims = reinterpret_cast<ImData*>(data_access->getGlobal(m_str_ims));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(m_str_pts));
	return (m_ims != 0 && m_pts != 0);
};

bool AcqImBlk::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << " RUN ims " << m_ims->DataDQ().size() << std::endl;
	// save out a pix pt for this run
	PixPt pt(m_val1);
	m_pts->PixPts().push_back(pt);
	return(true);
};

void AcqImBlk::doPrint() 
{ 
	std::cout << m_name << "  " << m_str_ims << " " << m_str_pts << std::endl;
	if(m_ims != NULL) m_ims->print();
	if(m_pts != NULL) m_pts->print();
	std::cout << std::endl;
};

//BOOST_CLASS_EXPORT(AcqImBlk);


