// Boost Circular Buffer bounded_buffer object
// Copyright (c) 2013 Perform3-D LLC

#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#define BOOST_CB_DISABLE_DEBUG

#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

// Circular Buffer which is fed by source with overwriting if not read fast enough
// 

template <class T>
class one_way_queue {
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename boost::call_traits<value_type>::param_type param_type;

    explicit one_way_queue(size_type capacity) : m_unread(0), m_lost_count(0), m_container(capacity) {}

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
        m_not_empty.wait(lock, boost::bind(&one_way_queue<value_type>::is_not_empty, this));
        *pItem = m_container.back();
		m_container.pop_back();
        lock.unlock();
    }
	unsigned getLostCount() { 
		boost::mutex::scoped_lock lock(m_mutex);
		return(m_lost_count); 
	}	// overwrites

private:
    one_way_queue(const one_way_queue&);              // Disabled copy constructor
    one_way_queue& operator = (const one_way_queue&); // Disabled assign operator

    bool is_not_empty() const { return !m_container.empty(); }
    bool is_empty() const { return m_container.empty(); }

    size_type m_unread;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
	unsigned m_lost_count;
};

// original boost circular buffer "bounded buffer" example
// synced tranfer with waiting if buffer full.
//

template <class T>
class bounded_buffer {
public:

    typedef boost::circular_buffer<T> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename boost::call_traits<value_type>::param_type param_type;

    explicit bounded_buffer(size_type capacity) : m_unread(0), m_container(capacity) {}

    void push_front(param_type item) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_full.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_full, this));
        m_container.push_front(item);
        ++m_unread;
        lock.unlock();
        m_not_empty.notify_one();
    }

    void pop_back(value_type* pItem) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_not_empty.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
        *pItem = m_container[--m_unread];
        lock.unlock();
        m_not_full.notify_one();
    }

private:
    bounded_buffer(const bounded_buffer&);              // Disabled copy constructor
    bounded_buffer& operator = (const bounded_buffer&); // Disabled assign operator

    bool is_not_empty() const { return m_unread > 0; }
    bool is_not_full() const { return m_unread < m_container.capacity(); }

    size_type m_unread;
    container_type m_container;
    boost::mutex m_mutex;
    boost::condition m_not_empty;
    boost::condition m_not_full;
};


#endif

