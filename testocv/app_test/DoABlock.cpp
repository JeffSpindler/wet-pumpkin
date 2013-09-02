
#include "DoABlock.h"

bool DoABlock::doSetup(IGlobalAccess *data_access) { 
	if(!data_access)	return false;

	m_D1 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D1));
	m_D2 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D2));
	return (m_D1 != 0 && m_D2 != 0);
}

bool DoABlock::doRun(IGlobalAccess *data_access) { 
	std::cout << m_name << std::endl;
	return(true);
}

void DoABlock::doPrint() { 
	std::cout << m_name << "  " << m_str_D1 << " " << m_str_D2 << std::endl;
	if(m_D1 != NULL) m_D1->print();
	if(m_D2 != NULL) m_D2->print();
	std::cout << std::endl;
}

BOOST_CLASS_EXPORT(DoABlock);


