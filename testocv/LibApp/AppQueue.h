// circular buffer which discards oldest values
// main app interface buffer
//
// Derived from Boost Circular Buffer bounded_buffer object
//
// Copyright (c) 2013 Perform3-D LLC

#ifndef APPQUEUE_H
#define APPQUEUE_H

#define BOOST_CB_DISABLE_DEBUG

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "Geom3d.h"

// Circular Buffer which is fed by source with overwriting if not read fast enough
// 

template <class T>
class AppQueue {
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename boost::call_traits<value_type>::param_type param_type;

    explicit AppQueue(size_type capacity) : m_lost_count(0), m_container(capacity) {}

	void init() {		// setup for restart
        boost::mutex::scoped_lock lock(m_mutex);
		m_container.clear();
		m_lost_count = 0;
	}

    void push_front(param_type item) {
        boost::mutex::scoped_lock lock(m_mutex);
		if(m_container.full()) ++m_lost_count;
        m_container.push_front(item);
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&AppQueue<value_type>::is_not_empty, this));
        *pItem = m_container.back();
		m_container.pop_back();
        lock.unlock();
    }
	unsigned getLostCount() { 
		boost::mutex::scoped_lock lock(m_mutex);
		return(m_lost_count); 
	}	// overwrites

private:
    AppQueue(const AppQueue&);              // Disabled copy constructor
    AppQueue& operator = (const AppQueue&); // Disabled assign operator

    bool is_not_empty() const { return !m_container.empty(); }
    bool is_empty() const { return m_container.empty(); }

    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
	unsigned m_lost_count;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		// do for each and write/read out elements ??
		// copy to a vector or read a vector
		//ar & BOOST_SERIALIZATION_NVP(m_container);
	}
};

typedef AppQueue<Geom3d> G3d_qu_t;


#endif

