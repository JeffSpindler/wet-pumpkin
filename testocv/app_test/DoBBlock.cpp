
#include "DoBBlock.h"
	
// Test class for logic block

bool DoBBlock::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;

	m_D3 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D3));
	m_D2 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D2));
	return (m_D3 != 0 && m_D2 != 0);
}

bool DoBBlock::doRun(IGlobalAccess *data_access) 
{ 
	std::cout << m_name << std::endl;
	return(true);
}

void DoBBlock::doPrint() 
{ 
	std::cout << m_name << "  " << m_str_D3 << " " << m_str_D2 << std::endl;
	if(m_D3 != NULL) m_D3->print();
	if(m_D2 != NULL) m_D2->print();
	std::cout << std::endl;
}

BOOST_CLASS_EXPORT(DoBBlock);


