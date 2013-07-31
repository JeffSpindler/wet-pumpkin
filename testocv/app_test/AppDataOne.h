#ifndef APPDATAONE_H
#define APPDATAONE_H

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
	AppDataOne() : m_read_only(false) { m_name = "AppDataOne"; };
	virtual ~AppDataOne() {};

public:		// public data
	bool m_read_only;
	std::string m_str_stuff;
	int m_value_int;
	float m_value_float;
	bool m_value_bool;

private:	// private data
	std::deque<int> m_test_int_dq;

private:	// access

	virtual void doInit()  {
		if(!m_read_only) {
			m_test_int_dq.clear(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " " << m_str_stuff << "  " << m_value_int << std::endl;
	}

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_test_int_dq);
	}
};

#endif


