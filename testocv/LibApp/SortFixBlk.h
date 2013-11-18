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
#include "CamRay.h"


#include "PairSort2MISet.h"
	
// Class for logic block

class SortFixBlk : public IBlock
{
public:
	SortFixBlk() :  m_cals(NULL), m_pts(NULL),
					m_fix(NULL), m_camrays(NULL), 
					m_model(NULL), m_cur(NULL), 
					m_val1(0), m_val2(0)  {
		m_name = "SortFixBlk";
	};
	virtual ~SortFixBlk() {};

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

	// local vars
	int m_num_cams;
	ps2_mi_set m_ray_pair_set;	// multi-index set for rays

	float m_angle_err_limit;	// rays must match fix within this
	int m_val1;
	float m_val2;

	// local functions
	bool createRayPairs(CamRay_v_t &camray_v);	// setup ray mi set
	bool findBestRays(CamRay_v_t &camray_v);	// set m_cur with result

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
		//ar & BOOST_SERIALIZATION_NVP(m_angle_err_limit);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_VERSION(SortFixBlk, 0)

#endif

