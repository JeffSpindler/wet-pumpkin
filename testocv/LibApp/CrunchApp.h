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
#include "AcqImBlk.h"
#include "PixCandsBlk.h"

#include "PixPtData.h"
#include "ImData.h"
#include "CamCalData.h"

#include "GlobalAccess.h"


// originate Geom3d objects using config from filename
// for test generate an g3d every second for 10 seconds
class Geom3d_Producer {

    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_count;
	static const int max_count = 5;
	AppSeq *m_seq;
	GlobalAccess *m_data_access;

public:
    Geom3d_Producer(std::string filename, GlobalAccess *data_access, G3d_qu_t* out_qu ) : m_out_qu(out_qu), 
																			m_run_flag(false), m_count(0) {
		m_data_access = data_access;
		// add output queue to data acces
		m_seq = SeqCreate(filename);
		};

    void operator()() {
		// first setup the seq object
		if(m_seq == NULL) { 
			std::cout << "Geom3d_Producer  INVALID SEQ\n\n";
			return;
		}
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
			std::cout << file_name << "SeqCreate " << e.what() << '\n';
			return(NULL);
		}
		return(new_seq);
	};
};

class Geom3d_Consumer {

    G3d_qu_t* m_in_qu;
    Geom3d m_item;
	bool m_run_flag;
	int m_count;
	AppSeq *m_seq;
	GlobalAccess *m_data_access;

public:
    Geom3d_Consumer(std::string filename, GlobalAccess *data_access, G3d_qu_t* in_qu) : 
						m_in_qu(in_qu), m_run_flag(false), m_count(0) {
		m_data_access = data_access;
		m_seq = SeqCreate(filename);
		};
    void operator()() {
		// first setup the seq object
		if(m_seq == NULL) { 
			std::cout << "Geom3d_Consumer  INVALID SEQ\n\n";
			return;
		}
		m_seq->print();
		m_seq->setup(m_data_access);
		//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		m_run_flag = true;
		while(m_run_flag) {
            m_in_qu->pop_back(&m_item);
			std::cout << m_count << " RECV " << m_item << std::endl;
			m_count++;
			m_seq->run(m_data_access);

			//boost::this_thread::sleep(boost::posix_time::milliseconds(500));
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
			std::cout << file_name << "SeqCreate " << e.what() << '\n';
			return(NULL);
		}
		return(new_seq);
	};

};



class CrunchApp
{
public:
    // This is based on an image object and a tag object with addl data required for use.
	//

    // Construction and destruction.
    CrunchApp ()  : m_valid(true), m_name("CrunchApp"), m_tag(0), m_idx(0), 
					m_time_usec(0), m_conf(0) {};
    CrunchApp (std::string &name) : m_valid(true), m_name(name), 
									m_tag(0), m_idx(0), m_time_usec(0), m_conf(0) {};

	virtual ~CrunchApp () {};

	bool onInitialize() { 
		static const std::string PixAppQueue = "PixAppQueue";
		static const std::string RawAppQueue = "RawAppQueue";

		std::cout << "onInitialize " << m_name << '\n';
		// create data managers for geom processing seqs -- shared
		//GlobalAccess data_access;
		GlobalAccess *m_data_access = setup_pix_seq_data();

		m_pix_data_qu = new AppQueueData(pix_queue_size);
		m_traj_data_qu = new AppQueueData(pix_queue_size);

		// make queue for pix geom objects
		if(!m_data_access->addGlobal(PixAppQueue, m_pix_data_qu)) printf("bad PixAppQueue\n");
		// make queue for raw geom objects
		if(!m_data_access->addGlobal(RawAppQueue, m_traj_data_qu)) printf("bad RawAppQueue\n");

		std::cout << "One Way Queues<Geom3d> ";

		std::cout << "Make Threads " << std::endl;

		std::string pix_file = "pix_seq.xml";
		std::string traj_file = "traj_seq.xml";

		m_pix_g3d_producer = new Geom3d_Producer(pix_file, m_data_access, &m_pix_data_qu->GeomQu());
		m_traj_g3d_consumer = new Geom3d_Consumer(traj_file, m_data_access, &m_pix_data_qu->GeomQu());

		// Start the threads.
		m_traj_g3d_consume = new boost::thread (*m_traj_g3d_consumer);

		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

		m_pix_g3d_produce = new boost::thread (*m_pix_g3d_producer);

		//while(1);
		std::cout << "Threads Active " << std::endl;	
		m_valid = true;
		return(m_valid); 
	};

	void OnTerminate() {
		if(!m_valid)
			return;

		// stop the threads
		m_traj_g3d_consume->detach();
		m_pix_g3d_produce->detach();
		// deallocate any objects
		delete m_traj_g3d_consume;
		delete m_pix_g3d_produce;
		delete m_pix_g3d_producer;
		delete m_traj_g3d_consumer;
		delete m_pix_data_qu;
		delete m_traj_data_qu;

		//delete m_data_access;
		std::cout << "OnTerminate " << m_name << '\n';
	
	};
	GlobalAccess *setup_pix_seq_data() { 
		static const std::string RawPixPts = "RawPixPts";
		static const std::string FinalPixPts = "FinalPixPts";
		static const std::string InputIms = "InputIms";
		static const std::string CamCals = "CamCals";
		static const std::string PixGeomsOut = "PixGeomsOut";

		GlobalAccess *data_access = new GlobalAccess;
		// raw pix pts storage
		PixPtData *raw_pts = new PixPtData;
		raw_pts->m_parm1 = 13;
		raw_pts->m_parm2 = 11;
		if(!data_access->addGlobal(RawPixPts, raw_pts)) printf("bad data1\n");
		// final pix pts 
		PixPtData *final_pts = new PixPtData;
		final_pts->m_parm1 = 23;
		final_pts->m_parm2 = 22;
		if(!data_access->addGlobal(FinalPixPts, final_pts)) printf("bad data1\n");
		// dummy ims
		ImData *input_ims = new ImData;
		input_ims->m_parm1 = 33;
		input_ims->m_parm2 = 33;
		if(!data_access->addGlobal(InputIms, input_ims)) printf("bad data2\n");
		// dummy cal
		CamCalData *cam_cal = new CamCalData;
		cam_cal->m_parm1 = 33;
		cam_cal->m_parm2 = 33;
		if(!data_access->addGlobal(CamCals, cam_cal)) printf("bad data2\n");
		// output geom3ds
		Geom3dData *geoms = new Geom3dData;
		if(!data_access->addGlobal(PixGeomsOut, geoms)) printf("bad data2\n");

		return(data_access);
	};
	// I/O methods
	bool addInput(Geom3d &g3d) {  // put a value in input que
		m_pix_data_qu->GeomQu().push_front(g3d);
		return(true);
	};
	

	bool m_valid;
	std::string m_name;
	int m_tag;
	int m_idx;
	double m_time_usec;
	double m_conf;

	// app data
	// create global shared data for geom processing seqs -- shared
	GlobalAccess *m_data_access;

	// make queue for pix geom objects
	AppQueueData *m_pix_data_qu;
	// make queue for raw geom objects
	AppQueueData *m_traj_data_qu;

    Geom3d_Producer *m_pix_g3d_producer;
    boost::thread *m_pix_g3d_produce;

    Geom3d_Consumer *m_traj_g3d_consumer;
    boost::thread *m_traj_g3d_consume;


	friend std::ostream& operator<<(std::ostream& os, const CrunchApp &pt) {
		if(!pt.m_valid) {
			os << " invalid\n";
			return os;
		}
		os.precision(1);
		os.setf(std::ios::fixed);
		os << std::setw(4) << "CrunchApp  " << pt.m_name << std::endl;
		//pt.m_tag << std::setw(4) << pt.m_idx << 
		//		" [" << pt.m_width << "x" << pt.m_height << "]" << std::endl; 
		return os;
	}
public:
	static const std::string PixAppQueueStr;
	static const std::string RawAppQueueStr;
	static const std::string RawPixPtsStr;
	static const std::string FinalPixPtsStr;
	static const std::string InputImsStr;
	static const std::string CamCalsStr;
	static const std::string PixGeomsOutStr;

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
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_idx );
		ar & BOOST_SERIALIZATION_NVP( m_time_usec );
		ar & BOOST_SERIALIZATION_NVP( m_conf );
	}
};

BOOST_CLASS_VERSION(CrunchApp, 0)


#endif
