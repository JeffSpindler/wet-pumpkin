// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/27)
//
// Geometric 3d Processing App -- Create main threads and Queue
//
#ifndef GEOMAPP_H
#define GEOMAPP_H

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <iomanip>

#include <boost/foreach.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#define BOOST_CB_DISABLE_DEBUG
#include <boost/circular_buffer.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

#include "Im.h"
#include "Pt3d.h"
#include "Geom3d.h"
#include "Geom3dData.h"
#include "AppQueueData.h"
#include "AppSeq.h"

#include "GlobalAccess.h"

using namespace P3D;
//namespace P3D
//{

// originate Geom3d objects using config from filename
// for test generate an g3d every second for 10 seconds
class G3d_Producer {

    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_count;
	static const int max_count = 5;
	AppSeq *m_seq;
	GlobalAccess *m_data_access;

public:
    G3d_Producer(std::string filename, GlobalAccess *data_access, G3d_qu_t* out_qu ) : m_out_qu(out_qu), 
																			m_run_flag(false), m_count(0) {
		m_data_access = data_access;
		// add output queue to data acces

		m_seq = SeqCreate(filename);
		};

    void operator()() {
		// first setup the seq object
		if(m_seq == NULL) return;
		m_seq->print();
		m_seq->setup(m_data_access);
		std::string Geom3dStr = "PixGeomsOut";
		Geom3dData *geoms = reinterpret_cast<Geom3dData*>(m_data_access->getGlobal(Geom3dStr));

		// now check for input and stuff objects
		m_run_flag = true;
		while(m_run_flag) {
			// check for new input
			if(m_count < max_count) {
				m_seq->run(m_data_access);
				// create a pt from the seq
				int idx_count = 0;
				BOOST_FOREACH(Geom3d pt, geoms->Geoms()) {
					// put in geom3d out
					//g3d.m_tag = pt.m_tag;
					//m_geoms->Geoms().push_back(g3d);
					pt.m_tag = m_count;
					pt.m_idx = idx_count++;
					std::cout << m_count << " SEND " << pt << std::endl;
					m_out_qu->push_front(pt);
				}
				geoms->Geoms().clear();
				m_count++;
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			}
		}
    }

	// create app seq from file
	AppSeq* SeqCreate(std::string &file_name) {
		AppSeq *new_seq;
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "SeqCreate " << "Bad File Open " << file_name << '\n';
				return(NULL);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(new_seq);
		} 
		catch(std::exception &e) {
			std::cout << "SeqCreate " << e.what() << '\n';
			return(NULL);
		}
		return(new_seq);
	};
};

class G3d_Consumer {

    G3d_qu_t* m_in_qu;
    Geom3d m_item;
	bool m_run_flag;
	int m_count;
	AppSeq *m_seq;
	GlobalAccess *m_data_access;

public:
    G3d_Consumer(std::string filename, GlobalAccess *data_access, G3d_qu_t* in_qu) : 
						m_in_qu(in_qu), m_run_flag(false), m_count(0) {
		m_data_access = data_access;
		m_seq = SeqCreate(filename);
		};
    void operator()() {
		// first setup the seq object
		m_seq->print();
		m_seq->setup(m_data_access);
		//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		m_run_flag = true;
		while(m_run_flag) {
            m_in_qu->pop_back(&m_item);
			std::cout << m_count << " RECV " << m_item << std::endl;
			m_count++;
			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        }
    }

	// create app seq from file
	AppSeq* SeqCreate(std::string &file_name) {
		AppSeq *new_seq;
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "SeqCreate " << "Bad File Open " << file_name << '\n';
				return(NULL);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(new_seq);
		} 
		catch(std::exception &e) {
			std::cout << "SeqCreate " << e.what() << '\n';
			return(NULL);
		}
		return(new_seq);
	};

};


class GeomApp // : boost::noncopyable
{
public:
    // Construction and destruction.
    GeomApp ();
    //GeomApp (std::string &name);
	virtual ~GeomApp ();

	bool onInitialize();
	void OnTerminate();
	GlobalAccess *GeomApp::setup_pix_seq_data();

	bool m_valid;
	std::string m_name;

	// app data
	// create global shared data for geom processing seqs -- shared
	GlobalAccess *m_data_access;

	//// make queue for pix geom objects
	//AppQueueData *m_pix_data_qu;
	//// make queue for raw geom objects
	//AppQueueData *m_raw_data_qu;

 //   G3d_Producer *m_pix_g3d_producer;
 //   G3d_Consumer *m_traj_g3d_consumer;

 //   boost::thread *m_traj_g3d_consume;
 //   boost::thread *m_pix_g3d_produce;



	friend std::ostream& operator<<(std::ostream& os, const GeomApp &app) {
		if(!app.m_valid) {
			os << " invalid\n";
			return os;
		}
		//os.precision(1);
		//os.setf(std::ios::fixed);
		os << app.m_name << std::endl;
		// os << std::setw(4) << g3d.m_tag << std::setw(4) << g3d.m_idx << 
		//		" (" << g3d.m_pt[0] << "," << g3d.m_pt[1] << "," << g3d.m_pt[2] << ")" << std::endl; 
		return os;
	}
	static const std::string PixAppQueue;
	static const std::string RawAppQueue;
	static const std::string RawPixPts;
	static const std::string FinalPixPts;
	static const std::string InputIms;
	static const std::string CamCals;
	static const std::string PixGeomsOut;

	static const int pix_queue_size = 10;
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
	}
};


//}
BOOST_CLASS_VERSION(GeomApp, 0)

#endif
