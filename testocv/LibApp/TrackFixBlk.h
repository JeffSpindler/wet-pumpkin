// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef TRACKFIXBLK_H
#define TRACKFIXBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

	
// Class for logic block

class TrackFixBlk : public IBlock
{
public:
	TrackFixBlk() :  m_cals(NULL), m_pts(NULL),
					m_geoms(NULL), m_val1(0),
						m_val2(0)  {
		m_name = "TrackFixBlk";
	};
	virtual ~TrackFixBlk() {};

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
	static bool Read(std::string &file_name, TrackFixBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "TrackFixBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "TrackFixBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(TrackFixBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "TrackFixBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "TrackFixBlk Write " << e.what() << '\n';
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

BOOST_CLASS_VERSION(TrackFixBlk, 0)

#endif

