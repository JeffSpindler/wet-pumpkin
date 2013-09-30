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
// pix blocks
#include "AcqImBlk.h"
#include "PixCandsBlk.h"
// traj blocks
#include "LoadFixBlk.h"
#include "SortFixBlk.h"
#include "LabelFixBlk.h"
#include "TrackFixBlk.h"

#include "PixPtData.h"
#include "ImData.h"
#include "CamCalData.h"

#include "GlobalAccess.h"


// originate Geom3d objects using config from filename
// for test generate an g3d every second for 10 seconds
class Geom3d_Processor {
public:
	G3d_qu_t* m_in_qu;
    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_count;
	AppSeq *m_seq;
	GlobalAccess *m_data_access;
	Geom3d_dq_t m_item;
	Geom3dData *m_camrays;
	static const int max_count = 10;

public:
    Geom3d_Processor(std::string filename, GlobalAccess *data_access, 
						G3d_qu_t* in_qu, G3d_qu_t* out_qu ) : m_in_qu(in_qu), m_out_qu(out_qu), 
																			m_run_flag(false), m_count(0) {
		m_data_access = data_access;
		m_camrays = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CamRayStr));
		// add output queue to data acces
		m_seq = SeqCreate(filename);
		};

    void operator()() {
		// first setup the seq object
		if(m_seq == NULL) { 
			std::cout << "Geom3d_Processor  INVALID SEQ\n\n";
			return;
		}
		m_seq->print();
		m_seq->setup(m_data_access);
		// now check for input and stuff objects
		m_run_flag = true;
		while(m_run_flag) {
			// check for new input
			m_camrays->Geoms().clear();
			m_in_qu->pop_back(&m_item);		
			BOOST_FOREACH(Geom3d g3d, m_item) {
				m_camrays->Geoms().push_back(g3d);
			}			
			if(Debug(0)) {
				std::cout << m_count << " Run with " << m_camrays->Geoms().size() << " camrays" << std::endl;
			}

			m_seq->run(m_data_access);
			// create a pt from the seq
			int idx_count = 0;

			// push the result
			m_out_qu->push_front(m_item);
			m_count++;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
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
protected:
	int m_debug;	// debug level
public:
	int getDebug() { return(m_debug); };
	int setDebug(int dLevel) { int old_debug = m_debug;
		m_debug = dLevel; return(m_debug); };
	bool Debug(int Level) { return(m_debug >= Level); };
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

	bool onInitialize();
	void OnTerminate();

	GlobalAccess *setup_global_data();
	bool testPixSeq(int mode = 0);
	bool testTrajSeq(int mode = 0);	
	bool testCamRay(int frame_num, Geom3d_v_t &model_v, Geom3d_dq_t &g3d_dq);
	bool testModel(int frame_num, Geom3d_v_t &model_v);

	// I/O methods
	bool addInput(Geom3d_dq_t &g3d) {  // put a value in input que
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
	// make queue for input pix geom objects
	AppQueueData *m_pix_data_qu;
	// make queue for result geom objects
	AppQueueData *m_traj_data_qu;
    Geom3d_Processor *m_traj_g3d_processor;		// traj seq processing
    boost::thread *m_traj_g3d_process;


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
