#ifndef IMDATA_H
#define IMDATA_H

#include <string>
#include <vector>
#include <deque>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "IGlobal.h"
#include "Im.h"


// Global Im dq

class ImData : public IGlobal
{
public:
	ImData() : m_read_only(false),
					m_parm1(0),
					m_parm2(0)
	{ 
			m_name = "ImData"; 
	};
	virtual ~ImData() {};

public:		// public data
	Im_dq_t& DataDQ() { return(m_im_dq); };
	bool m_read_only;
	int m_parm1;
	float m_parm2;

private:	// private data
	Im_dq_t m_im_dq;

private:	// access

	virtual void doSetup()  {
		if(!m_read_only) {
			m_im_dq.clear(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " size " << m_im_dq.size() << "  " << m_parm1 << "  " << m_parm2 << std::endl;
	};

public:
	static bool Read(std::string &file_name, ImData *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "ImData Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "ImData Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(ImData *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "ImData Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "ImData Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP(m_im_dq);
	}
};

BOOST_CLASS_VERSION(ImData, 0)

#endif


