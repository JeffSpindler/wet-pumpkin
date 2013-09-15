// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef LABELFIXBLK_H
#define LABELFIXBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

#include "Geom3dMISet.h"

	
// Class for logic block

class LabelFixBlk : public IBlock
{
public:
	LabelFixBlk() :  m_cals(NULL), m_pts(NULL),
						m_fix(NULL), m_camrays(NULL), 
						m_model(NULL), m_cur(NULL), 
						m_val1(0),	m_val2(0)  {
		m_name = "LabelFixBlk";
	};
	virtual ~LabelFixBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

protected:
	Geom3dData *m_camrays;
	Geom3dData *m_fix;
	Geom3dData *m_model;
	Geom3dData *m_cur;

	CamCalData *m_cals;
	PixPtData *m_pts;

	g3d_mi_set m_ray_set;	// multi-index set for rays
	//g3d_mi_set m_g3d_set;	// multi-index set for results
	
	int m_val1;
	float m_val2;

protected:
	bool LabelFixBlk::intersectRays();	// make cur geoms


public:
	static bool Read(std::string &file_name, LabelFixBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "LabelFixBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "LabelFixBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(LabelFixBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "LabelFixBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "LabelFixBlk Write " << e.what() << '\n';
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

BOOST_CLASS_VERSION(LabelFixBlk, 0)

#endif

