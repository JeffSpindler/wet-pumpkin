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

typedef one_way_queue<P3D::Pt3d> Pt3d_qu_t;

const unsigned QUEUE_SIZE     = 50;
const unsigned NUM_INIT = QUEUE_SIZE*2 - 1;
const unsigned NUM_SEND = QUEUE_SIZE*2;

class Pt3d_Consumer {

    Pt3d_qu_t* m_container;
    Pt3d m_item;
	bool m_run_flag;
	int m_count;

public:
    Pt3d_Consumer(Pt3d_qu_t* buffer) : m_container(buffer), m_run_flag(false), m_count(0) {}

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

class Pt3d_Producer {

    Pt3d_qu_t* m_container;
	bool m_run_flag;
	int m_count;

public:
    Pt3d_Producer(Pt3d_qu_t* buffer) : m_container(buffer), m_run_flag(false), m_count(0) {}

    void operator()() {
		Pt3d pt;
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

void fifo_test(Pt3d_qu_t* buffer) {

	std::cout << "Start Up " << std::endl;
	Pt3d pt;

    // Start of measurement
    boost::progress_timer progress;

    // Initialize the buffer with some values before launching producer and consumer threads.
    for (unsigned long i = 0; i < NUM_INIT; i++) {
		pt.m_tag = i;
		std::cout << " Start " << pt << std::endl;
        buffer->push_front(pt);
    }
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));

	std::cout << "Make Threads " << std::endl;

    Pt3d_Consumer pt3d_consumer(buffer);
    Pt3d_Producer pt3d_producer(buffer);

    // Start the threads.
    boost::thread pt3d_consume(pt3d_consumer);
	
	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    boost::thread pt3d_produce(pt3d_producer);

    // Wait for completion.

	std::cout << "Wait for Producer Done  Lost " << buffer->getLostCount() << std::endl;

    pt3d_produce.join();
	std::cout << "Producer Done  Lost " << buffer->getLostCount() << std::endl;

    pt3d_consume.join();
	std::cout << "All Done Lost " << buffer->getLostCount() << std::endl;


    // End of measurement
}


void app_startup();
int seq_test_main( void );

int main(int argc, char* argv[]) {

    //seq_test_main();

	app_startup();

	std::cout << "\n\nBack from app -- STuff test values\n\n";

	//while(1);

	return 0;
}
