// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef ACQIMBLK_H
#define ACQIMBLK_H

#include "Global.h"

#include "IBlock.h"

#include "ImData.h"
#include "PixPtData.h"

#include "IGlobalAccess.h"
	
// Test class for logic block

class AcqImBlk : public IBlock
{
public:
	AcqImBlk() :
		m_ims(NULL),
		m_pts(NULL),
		m_val1(0),
		m_val2(0)
	{
		m_name = "AcqImBlk";
	};
	virtual ~AcqImBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

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
	template<class Archive>	void serialize(Archive& ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_str_ims);
		ar & BOOST_SERIALIZATION_NVP(m_str_pts);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_VERSION(AcqImBlk, 0)


#endif

