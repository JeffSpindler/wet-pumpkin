#ifndef PIXCANDSBLK_H
#define PIXCANDSBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"
	
// Test class for logic block

class PixCandsBlk : public IBlock
{
public:
	PixCandsBlk() :  m_cals(NULL), m_pts(NULL),
						m_geoms(NULL), m_val1(0),
						m_val2(0)  {
		m_name = "PixCandsBlk";
	};
	virtual ~PixCandsBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

protected:
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
	template<class Archive>	void serialize(Archive& ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};
BOOST_CLASS_VERSION(PixCandsBlk, 0)

#endif

