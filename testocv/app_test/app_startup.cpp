// Skeleton App with queues and threads

// Copyright (c) 2013 Perform3-D LLC

#define BOOST_CB_DISABLE_DEBUG

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>
#include <iostream>

#include "bounded_buffer.h"

#include "Pt3d.h"
#include "Geom3d.h"

using namespace P3D;

//typedef one_way_queue<P3D::Pt3d> Pt3d_qu_t;
typedef one_way_queue<P3D::Geom3d> G3d_qu_t;

const unsigned QUEUE_SIZE     = 50;
const unsigned NUM_INIT = QUEUE_SIZE*2 - 1;
const unsigned NUM_SEND = QUEUE_SIZE*2;

class G3d_Consumer {

    G3d_qu_t* m_container;
    Geom3d m_item;
	bool m_run_flag;
	int m_count;

public:
    G3d_Consumer(G3d_qu_t* buffer) : m_container(buffer), m_run_flag(false), m_count(0) {}

    void operator()() {
		//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		m_run_flag = true;
		while(m_run_flag) {
            m_container->pop_back(&m_item);
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
			std::cout << m_count << " RECV " << m_item << std::endl;
			m_count++;
        }
    }
};

class G3d_Producer {

    G3d_qu_t* m_container;
	bool m_run_flag;
	int m_count;

public:
    G3d_Producer(G3d_qu_t* buffer, std::string filename ) : m_container(buffer), m_run_flag(false), m_count(0) {}

    void operator()() {
		Geom3d pt;
		m_run_flag = true;
		//while(m_run_flag) {
        for (unsigned long i = 0L; i < NUM_SEND; ++i) {
			pt.m_tag = 100+i;
            m_container->push_front(pt);
			std::cout << m_count << " SEND " << pt << std::endl;
			m_count++;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
    }
};

class G3d_Transformer {

    G3d_qu_t* m_in_qu;
    G3d_qu_t* m_out_qu;
	bool m_run_flag;
	int m_count;

public:
    G3d_Transformer(G3d_qu_t* in_qu, G3d_qu_t* out_qu) : m_container(in_qu), m_run_flag(false), m_count(0) {}

    void operator()() {
		Geom3d pt;
		m_run_flag = true;
		//while(m_run_flag) {
        for (unsigned long i = 0L; i < NUM_SEND; ++i) {
			pt.m_tag = 100+i;
            m_container->push_front(pt);
			std::cout << m_count << " SEND " << pt << std::endl;
			m_count++;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
    }
};


void app_startup() {

	std::cout << "Start Up " << std::endl;
	Geom3d pt;

	// get filenames for geom processing seqs


	// create data managers for geom processing seqs


	// make queues for geom objects
	G3d_qu_t pix_g3d_qu(QUEUE_SIZE);
	G3d_qu_t raw_g3d_qu(QUEUE_SIZE);
	G3d_qu_t traj_g3d_qu(QUEUE_SIZE);

	std::cout << "One Way Queues<Gemo3d> ";

    // Start of measurement
    boost::progress_timer progress;

    // Initialize the buffer with some values before launching producer and consumer threads.
    for (unsigned long i = 0; i < NUM_INIT; i++) {
		pt.m_tag = i;
		std::cout << " Start " << pt << std::endl;
        pix_g3d_qu->push_front(pt);
    }
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));

	std::cout << "Make Threads " << std::endl;

	std::string pix_file;
	std::string raw_file;
	std::string traj_file;

    G3d_Producer pix_g3d_producer(pix_file, pix_g3d_qu);
    G3d_Transformer raw_g3d_transformer(pix_g3d_qu, raw_g3d_qu);
    G3d_Consumer traj_g3d_consumer(raw_g3d_qu);

    // Start the threads.
    boost::thread pix_g3d_produce(pix_g3d_producer);
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));

    boost::thread raw_g3d_transform(raw_g3d_transformer);

    boost::thread traj_g3d_consume(traj_g3d_consumer);




    // Wait for completion.

	std::cout << "Wait for Producer Done  Lost " << buffer->getLostCount() << std::endl;

    pt3d_produce.join();
	std::cout << "Producer Done  Lost " << buffer->getLostCount() << std::endl;

    pt3d_consume.join();
	std::cout << "All Done Lost " << buffer->getLostCount() << std::endl;


    // End of measurement
}

// read main file and get the necessary object inited
bool createMainApp()
{






}

