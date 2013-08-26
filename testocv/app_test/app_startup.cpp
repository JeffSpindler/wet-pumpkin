// Skeleton App with queues and threads

// Copyright (c) 2013 Perform3-D LLC

#define BOOST_CB_DISABLE_DEBUG

#include <string>
#include <iostream>

#include <boost/foreach.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

//#include "bounded_buffer.h"

#include "Pt3d.h"
#include "Geom3d.h"
#include "Geom3dData.h"
#include "AppQueueData.h"

#include "DoASeq.h"
#include "GlobalAccess.h"

using namespace P3D;

//typedef one_way_queue<P3D::Pt3d> Pt3d_qu_t;
//typedef one_way_queue<P3D::Geom3d> G3d_qu_t;

const unsigned QUEUE_SIZE     = 10;
const unsigned NUM_INIT = QUEUE_SIZE*2 - 1;
const unsigned NUM_SEND = QUEUE_SIZE*2;

DoASeq* SeqCreate(std::string &file_name)
{
	DoASeq *new_seq;
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
}

class G3d_Consumer {

    G3d_qu_t* m_in_qu;
    Geom3d m_item;
	bool m_run_flag;
	int m_count;
	DoASeq *m_seq;
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
};

// originate Geom3d objects using config from filename
// for test generate an g3d every second for 10 seconds
class G3d_Producer {

    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_count;
	static const int max_count = 5;
	DoASeq *m_seq;
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
};

class G3d_Transformer {

    G3d_qu_t* m_in_qu;
    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_in_count;
	int m_out_count;
	DoASeq *m_seq;
	GlobalAccess *m_data_access;

public:
    G3d_Transformer(std::string filename, GlobalAccess *data_access, G3d_qu_t* in_qu, G3d_qu_t* out_qu) : 
									m_in_qu(in_qu), m_out_qu(out_qu), 
									m_run_flag(false), m_in_count(0),
									m_out_count(0) {
		m_data_access = data_access;
		m_seq = SeqCreate(filename);
		};

    void operator()() {
		Geom3d pt;
		// first setup the seq object
		m_seq->print();
		m_seq->setup(m_data_access);
		m_run_flag = true;
		while(m_run_flag) {
            m_in_qu->pop_back(&pt);
			std::cout << m_in_count << " TRECV " << pt << std::endl;
			m_in_count++;

			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			pt.m_tag += 900;
			std::cout << m_out_count << " TSEND " << pt << std::endl;
            m_out_qu->push_front(pt);
			m_out_count++;
		}
    }
};

GlobalAccess *setup_pix_seq_data();
void create_pix_seq();

std::string PixAppQueue = "PixAppQueue";
std::string RawAppQueue = "RawAppQueue";

void app_startup() 
{
	std::cout << "app_startup " << std::endl << std::endl;

	// get filenames for geom processing seqs
	create_pix_seq(); // save out a valid pix seq

	// create data managers for geom processing seqs -- shared
	//GlobalAccess data_access;
	GlobalAccess *data_access = setup_pix_seq_data();

	// make queue for pix geom objects
	AppQueueData pix_data_qu(QUEUE_SIZE);
	if(!data_access->addGlobal(PixAppQueue, &pix_data_qu)) printf("bad PixAppQueue\n");
	// make queue for raw geom objects
	AppQueueData raw_data_qu(QUEUE_SIZE);
	if(!data_access->addGlobal(RawAppQueue, &raw_data_qu)) printf("bad RawAppQueue\n");
	
	//G3d_qu_t raw_g3d_qu(QUEUE_SIZE);
	//G3d_qu_t traj_g3d_qu(QUEUE_SIZE);

	std::cout << "One Way Queues<Geom3d> ";

    // Start of measurement
    //boost::progress_timer progress;

	std::cout << "Make Threads " << std::endl;

	std::string pix_file = "pix_seq.xml";
	std::string raw_file = "raw_seq.xml";
	std::string traj_file = "traj_seq.xml";

    G3d_Producer pix_g3d_producer(pix_file, data_access, &pix_data_qu.GeomQu());
    G3d_Transformer raw_g3d_transformer(raw_file, data_access, &pix_data_qu.GeomQu(), &raw_data_qu.GeomQu());
    G3d_Consumer traj_g3d_consumer(traj_file, data_access, &raw_data_qu.GeomQu());

    // Start the threads.
    boost::thread raw_g3d_transform(raw_g3d_transformer);

    boost::thread traj_g3d_consume(traj_g3d_consumer);

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    boost::thread pix_g3d_produce(pix_g3d_producer);

	boost::this_thread::sleep(boost::posix_time::milliseconds(30000));
	//while(1);
	std::cout << "Exiting " << std::endl;
	return;

    pix_g3d_produce.detach();

    raw_g3d_transform.detach();

    traj_g3d_consume.detach();

	std::cout << "Exiting " << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

	return;


    // Wait for completion.

	/*
	std::cout << "Wait for Pix Done  Lost " << pix_g3d_qu.getLostCount() << std::endl;

    pix_g3d_produce.join();
	std::cout << "Pix Done  Lost " << pix_g3d_qu.getLostCount() << std::endl;

    raw_g3d_transform.join();
	std::cout << "raw Done Lost " << pix_g3d_qu.getLostCount() << std::endl;

    traj_g3d_consume.join();
	*/

	std::cout << "Wait forever " << std::endl;
	while(1);

	boost::this_thread::sleep(boost::posix_time::milliseconds(6000));
	std::cout << "Stuff test values " << std::endl;

    // Initialize the buffer with some values before launching producer and consumer threads.
    for (unsigned long i = 0; i < 5; i++) {
		Geom3d pt(100+i);
		std::cout << " Start " << pt << std::endl;
        pix_data_qu.GeomQu().push_front(pt);
    }

	std::cout << "Wait for 30 secs " << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(30000));
	//while(1);

    pix_g3d_produce.detach();

    raw_g3d_transform.detach();

    traj_g3d_consume.detach();

	std::cout << "Exiting " << std::endl;
	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

    // End of measurement
}

// read main file and get the necessary object inited
bool createMainApp()
{





	return(true);
}

