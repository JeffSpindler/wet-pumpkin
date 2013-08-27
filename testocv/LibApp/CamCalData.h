#ifndef CAMCALDATA_H
#define CAMCALDATA_H

#include <string>
#include <vector>
#include <deque>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "IGlobal.h"
#include "CamCal.h"


// Global Im dq

namespace P3D
{

class CamCalData : public IGlobal
{
public:
	CamCalData() : m_read_only(false),
					m_parm1(0),
					m_parm2(0)
	{ 
			m_name = "ImData"; 
	};
	virtual ~CamCalData() {};

public:		// public data
	camcal_v_t& CamData() { return(m_camcal_v); };
	bool m_read_only;
	int m_parm1;
	float m_parm2;

private:	// private data
	camcal_v_t m_camcal_v;

private:	// access

	virtual void doSetup()  {
		if(!m_read_only) {
			m_camcal_v.clear(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " size " << m_camcal_v.size() << "  " << m_parm1 << "  " << m_parm2 << std::endl;
	};

public:
	static bool Read(std::string &file_name, CamCalData *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "CamCalData Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "CamCalData Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(CamCalData *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "CamCalData Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "CamCalData Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IGlobal);
		ar & BOOST_SERIALIZATION_NVP( m_read_only );
		ar & BOOST_SERIALIZATION_NVP( m_parm1 );
		ar & BOOST_SERIALIZATION_NVP( m_parm2 );
		ar & BOOST_SERIALIZATION_NVP(m_camcal_v);
	}
};

}
BOOST_CLASS_VERSION(P3D::CamCalData, 0)



#endif


