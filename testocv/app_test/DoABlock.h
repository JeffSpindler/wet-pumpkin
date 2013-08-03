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
		m_D1(NULL),
		m_D2(NULL),
		m_str_D1("data1"),
		m_str_D2("data2"),
		m_val1(0),
		m_val2(0)
	{
		m_name = "DoABlock";
	};
	virtual ~DoABlock() {};

	bool doSetup(IGlobalAccess *data_access) { 
		if(!data_access)	return false;

		m_D1 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D1));
		m_D2 = reinterpret_cast<AppDataOne*>(data_access->getGlobal(m_str_D2));
		return (m_D1 != 0 && m_D2 != 0);
	};

	bool doRun(IGlobalAccess *data_access = NULL) { 
		std::cout << m_name << std::endl;
		return(true);
	};

	void doPrint() { 
		std::cout << m_name << "  " << m_str_D1 << " " << m_str_D2 << std::endl;
		if(m_D1 != NULL) m_D1->print();
		if(m_D2 != NULL) m_D2->print();
		std::cout << std::endl;
	};

protected:
	std::string m_str_D1;
	std::string m_str_D2;
	AppDataOne *m_D1;
	AppDataOne *m_D2;

	int m_val1;
	float m_val2;
public:
	static bool Read(std::string &file_name, DoABlock *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "DoABlock Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "DoABlock Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(DoABlock *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "DoABlock Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "DoABlock Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

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
BOOST_CLASS_VERSION(DoABlock, 0)
BOOST_CLASS_EXPORT(DoABlock);

#endif

