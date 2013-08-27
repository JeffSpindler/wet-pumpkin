#ifndef APPQUEUEDATA_H
#define APPQUEUEDATA_H

#include <string>
#include <vector>
#include <deque>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "IGlobal.h"
#include "Geom3d.h"
#include "AppQueue.h"


// Global Geom Vector

namespace P3D
{
class AppQueueData : public IGlobal
{
public:
	AppQueueData(int capacity = 10) : m_read_only(false), m_g3d_qu(capacity)
	{ 
			m_name = "AppQueueData"; 
	};
	virtual ~AppQueueData() {};

public:		// public data
	G3d_qu_t& GeomQu() { return(m_g3d_qu); };
	bool m_read_only;

private:	// private data
	G3d_qu_t m_g3d_qu;

private:	// access

	virtual void doSetup()  {
		if(!m_read_only) {
			m_g3d_qu.init(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " lost " << m_g3d_qu.getLostCount() << std::endl;
	};

public:
	static bool Read(std::string &file_name, AppQueueData *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "AppQueueData Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AppQueueData Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(AppQueueData *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "AppQueueData Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AppQueueData Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IGlobal);
		ar & BOOST_SERIALIZATION_NVP( m_read_only );
		//ar & BOOST_SERIALIZATION_NVP(m_g3d_qu);
	}
};

};

BOOST_CLASS_VERSION(P3D::AppQueueData, 0)



#endif


