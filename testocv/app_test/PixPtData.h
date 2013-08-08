#ifndef PixPtData_H
#define PixPtData_H

#include <vector>
#include <string>
#include <deque>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "IGlobal.h"


// Interface class for global managed data

class AppDataOne : public IGlobal
{
public:
	AppDataOne() : m_read_only(false),
					m_value_int(0),
					m_value_float(0),
					m_value_bool(false)
	{ 
			m_name = "AppDataOne"; 
	};
	virtual ~AppDataOne() {};

public:		// public data
	bool m_valid;
    int m_tag;
    int m_idx;
    float m_pt[2];
    float m_conf;

private:	// private data
	std::deque<int> m_test_int_dq;

private:	// access

	virtual void doSetup()  {
		if(!m_read_only) {
			m_test_int_dq.clear(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " " << m_str_stuff << "  " << m_value_int << std::endl;
	};

public:
	static bool Read(std::string &file_name, AppDataOne *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "AppDataOne Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AppDataOne Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(AppDataOne *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "AppDataOne Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AppDataOne Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP( m_str_stuff );
		ar & BOOST_SERIALIZATION_NVP( m_value_int );
		ar & BOOST_SERIALIZATION_NVP( m_value_float );
		ar & BOOST_SERIALIZATION_NVP( m_value_bool );
		ar & BOOST_SERIALIZATION_NVP(m_test_int_dq);
	}
};

BOOST_CLASS_VERSION(AppDataOne, 0)

#endif


