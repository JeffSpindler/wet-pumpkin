#ifndef PIXCANDSBLK_H
#define PIXCANDSBLK_H

#include <string>
#include <vector>
#include <map>

#include <boost/foreach.hpp>
#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

class IGlobalAccess;
	
// Test class for logic block
namespace P3D
{

class PixCandsBlk : public IBlock
{
public:
	PixCandsBlk() :
		m_cals(NULL),
		m_str_cals("CamCals"),
		m_pts(NULL),
		m_str_pts("RawPixPts"),
		m_geoms(NULL),
		m_str_geoms("PixGeomsOut"),
		m_val1(0),
		m_val2(0)
	{
		m_name = "PixCandsBlk";
	};
	virtual ~PixCandsBlk() {};

	bool doSetup(IGlobalAccess *data_access) { 
		if(!data_access)	return false;

		m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(m_str_cals));
		m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(m_str_pts));
		m_geoms = reinterpret_cast<Geom3dData*>(data_access->getGlobal(m_str_geoms));
		return (m_cals != NULL && m_pts != NULL && m_geoms != NULL);
	};

	bool doRun(IGlobalAccess *data_access = NULL) { 
		std::cout << m_name << " RUN size " << m_pts->PixPts().size() << std::endl;
		BOOST_FOREACH(PixPt pt, m_pts->PixPts()) {
			// put in geom3d out
			Geom3d g3d(pt.m_tag);
			m_geoms->Geoms().push_back(g3d);
		}
		m_pts->PixPts().clear();
		return(true);
	};

	void doPrint() { 
		std::cout << m_name << "  " << m_str_cals << " " << m_str_pts << std::endl;
		if(m_cals != NULL) m_cals->print();
		if(m_pts != NULL) m_pts->print();
		if(m_geoms != NULL) m_geoms->print();
		std::cout << std::endl;
	};

protected:
	std::string m_str_cals;
	std::string m_str_pts;
	std::string m_str_geoms;
	CamCalData *m_cals;
	PixPtData *m_pts;
	Geom3dData *m_geoms;

	int m_val1;
	float m_val2;
public:
	static bool Read(std::string &file_name, PixCandsBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "PixCandsBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "PixCandsBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(PixCandsBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "PixCandsBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "PixCandsBlk Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP(m_str_cals);
		ar & BOOST_SERIALIZATION_NVP(m_str_pts);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

};

BOOST_CLASS_VERSION(P3D::PixCandsBlk, 0)
BOOST_CLASS_EXPORT(P3D::PixCandsBlk);

#endif

