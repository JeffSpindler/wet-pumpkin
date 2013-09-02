// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef SORTFIXBLK_H
#define SORTFIXBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

	
// Class for logic block

class SortFixBlk : public IBlock
{
public:
	SortFixBlk() :  m_cals(NULL), m_str_cals("CamCals"), m_pts(NULL),
						m_str_pts("RawPixPts"), m_geoms(NULL),
						m_str_geoms("PixGeomsOut"), m_val1(0),
						m_val2(0)  {
		m_name = "SortFixBlk";
	};
	virtual ~SortFixBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

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
	static const std::string block_name;
	static bool Read(std::string &file_name, SortFixBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "SortFixBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "SortFixBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(SortFixBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "SortFixBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "SortFixBlk Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_str_cals);
		ar & BOOST_SERIALIZATION_NVP(m_str_pts);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_VERSION(SortFixBlk, 0)

#endif

