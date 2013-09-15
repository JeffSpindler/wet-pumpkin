#ifndef GEOM3DDATA_H
#define GEOM3DDATA_H

#include <string>
#include <vector>
#include <deque>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>

#include "IGlobal.h"
#include "Geom3d.h"


// Global Geom Vector

class Geom3dData : public IGlobal
{
public:
	Geom3dData() : m_read_only(false)
	{ 
			m_name = "Geom3dData"; 
	};
	virtual ~Geom3dData() {};

public:		// public data
	Geom3d_v_t& Geoms() { return(m_geom3d_v); };
	bool m_read_only;

private:	// private data
	Geom3d_v_t m_geom3d_v;

private:	// access

	virtual void doSetup()  {
		if(!m_read_only) {
			m_geom3d_v.clear(); 
		}
	};
	virtual void doPrint() {
		std::cout << m_name << " size " << m_geom3d_v.size() << std::endl;
		BOOST_FOREACH(Geom3d g3d, m_geom3d_v) {
			std::cout << g3d;
		}
	};

public:
	static bool Read(std::string &file_name, Geom3dData *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "Geom3dData Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "Geom3dData Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(Geom3dData *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "Geom3dData Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "Geom3dData Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP(m_geom3d_v);
	}
};

BOOST_CLASS_VERSION(Geom3dData, 0)

#endif


