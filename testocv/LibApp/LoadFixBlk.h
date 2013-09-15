// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef LOADFIXBLK_H
#define LOADFIXBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

	
// Class for logic block

class LoadFixBlk : public IBlock
{
public:
	LoadFixBlk() :  m_cals(NULL), m_pts(NULL), 
					m_fix(NULL), m_camrays(NULL), m_model(NULL), 
					m_val1(0), m_val2(0)  {
		m_name = "LoadFixBlk";
	};
	virtual ~LoadFixBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

protected:
	CamCalData *m_cals;
	PixPtData *m_pts;
	Geom3dData *m_camrays;
	Geom3dData *m_fix;
	Geom3dData *m_model;

	int m_val1;
	float m_val2;

public:
	static bool Read(std::string &file_name, LoadFixBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "LoadFixBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "LoadFixBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(LoadFixBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "LoadFixBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "LoadFixBlk Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_VERSION(LoadFixBlk, 0)

#endif

