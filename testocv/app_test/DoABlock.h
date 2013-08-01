#ifndef DOABLOCK_H
#define DOABLOCK_H

#include <string>
#include <vector>
#include <map>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "IBlock.h"
#include "AppDataOne.h"
#include "IGlobalAccess.h"

class IGlobalAccess;
	
// Test class for logic block

class DoABlock : public IBlock
{
public:
	DoABlock() : 
		m_str_D1("data1"),
		m_str_D2("data2")
	{
		m_name = "DoABlock";
	};
	virtual ~DoABlock() {};

	bool doInit(IGlobalAccess *data_access) { 
		if(!data_access)	return false;

		m_D1 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D1));
		m_D2 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D2));
		return (m_D1 != 0 && m_D2 != 0);
	};

	bool doRun(IGlobalAccess *data_access) { 
		std::cout << m_name << std::endl;
		return(true);
	};

	void doPrint() { 
	};

protected:
	std::string m_str_D1;
	std::string m_str_D2;
	AppDataOne *m_D1;
	AppDataOne *m_D2;

	int m_val1;
	float m_val2;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_str_D1);
		ar & BOOST_SERIALIZATION_NVP(m_str_D2);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_EXPORT(DoABlock);

#endif

