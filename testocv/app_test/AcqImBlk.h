#ifndef ACQIMBLK_H
#define ACQIMBLK_H

#include <string>
#include <vector>
#include <map>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "IBlock.h"

#include "ImData.h"
#include "PixPtData.h"

#include "IGlobalAccess.h"

class IGlobalAccess;
	
// Test class for logic block

namespace P3D
{
class AcqImBlk : public IBlock
{
public:
	AcqImBlk() :
		m_ims(NULL),
		m_pts(NULL),
		m_str_ims("InputIms"),
		m_str_pts("RawPixPts"),
		m_val1(0),
		m_val2(0)
	{
		m_name = "AcqImBlk";
	};
	virtual ~AcqImBlk() {};

	bool doSetup(IGlobalAccess *data_access) { 
		if(!data_access)	return false;
		m_val1 = 0;

		m_ims = reinterpret_cast<ImData*>(data_access->getGlobal(m_str_ims));
		m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(m_str_pts));
		return (m_ims != 0 && m_pts != 0);
	};

	bool doRun(IGlobalAccess *data_access = NULL) { 
		std::cout << m_name << " RUN ims " << m_ims->DataDQ().size() << std::endl;
		// save out a pix pt for this run
		PixPt pt(m_val1);
		m_pts->PixPts().push_back(pt);
		return(true);
	};

	void doPrint() { 
		std::cout << m_name << "  " << m_str_ims << " " << m_str_pts << std::endl;
		if(m_ims != NULL) m_ims->print();
		if(m_pts != NULL) m_pts->print();
		std::cout << std::endl;
	};

protected:
	std::string m_str_ims;
	std::string m_str_pts;
	ImData *m_ims;
	PixPtData *m_pts;

	int m_val1;
	float m_val2;
public:
	static bool Read(std::string &file_name, AcqImBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "AcqImBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "AcqImBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(AcqImBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "AcqImBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "AcqImBlk Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP(m_str_ims);
		ar & BOOST_SERIALIZATION_NVP(m_str_pts);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

};

BOOST_CLASS_VERSION(P3D::AcqImBlk, 0)
BOOST_CLASS_EXPORT(P3D::AcqImBlk);

#endif

