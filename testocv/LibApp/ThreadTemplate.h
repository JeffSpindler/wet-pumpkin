// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/27)

#ifndef CRUNCHAPP_H
#define CRUNCHAPP_H

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <iomanip>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/foreach.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

// do something using config from filename

class Template_Consumer {

	bool m_run_flag;
	int m_count;
	std::string m_name;

public:
    Template_Consumer(std::string filename) : m_run_flag(false), m_count(0), m_name(filename) {
		};
    void operator()() {
		// first setup the object
		m_run_flag = true;
		// now do forever -- interlocks are on the external data access
		while(m_run_flag) {
			std::cout << m_count << " HI from "  << m_name << std::endl;
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
			m_count++;
        }
    }
};

class TemplateApp
{
public:
    // This is based on an image object and a tag object with addl data required for use.
	//

    // Construction and destruction.
    TemplateApp ()  : m_valid(true), m_name("TemplateApp"), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};
    TemplateApp (std::string &name) : m_valid(true), m_name(name), 
									m_tag(0), m_idx(0), m_time_usec(0), m_conf(0) {};

	virtual ~TemplateApp () {};

	bool onInitialize() { 

		std::cout << "onInitialize " << m_name << '\n';

		std::cout << "Make Thread " << std::endl;

		std::string traj_file = "traj_seq.xml";

		m_test_consumer = new Template_Consumer(traj_file);
		// Start the threads.
		m_test_consume = new boost::thread (*m_test_consumer);

		std::cout << "Threads Active " << std::endl;	
		m_valid = true;
		return(m_valid); 
	};

	void OnTerminate() {
		if(!m_valid)
			return;

		// stop the threads
		m_test_consume->detach();
		// deallocate any objects
		delete m_test_consume;
		delete m_test_consumer;

		//delete m_data_access;
		std::cout << "OnTerminate " << m_name << '\n';
	
	};
	
	bool m_valid;
	std::string m_name;
	int m_tag;
	int m_idx;
	double m_time_usec;
	double m_conf;

	// app data
    Template_Consumer *m_test_consumer;


	friend std::ostream& operator<<(std::ostream& os, const TemplateApp &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << "Thread  " << pt.m_name << std::endl;
		return os;
	}
public:
	//static const std::string PixAppQueueStr;
	static const int raw_queue_size = 10;

	enum {
		INVALID = 0,
	};
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_valid );
		ar & BOOST_SERIALIZATION_NVP( m_name );
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_idx );
		ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

BOOST_CLASS_VERSION(CrunchApp, 0)


#endif
