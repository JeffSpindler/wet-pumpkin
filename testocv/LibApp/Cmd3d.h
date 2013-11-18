// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/11/18)
//
// Cmd Object
//
#ifndef CMD3D_H
#define CMD3D_H

#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <iomanip>

#include "boost/format.hpp"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>

class Cmd3d
{
public:
    // This has an int cmd, sub_cmd
	// paired with int parm, int src, float val
	//
    // Construction and destruction.
    Cmd3d () : m_cmd(0), m_sub_cmd(0), m_src(0), m_parm(0), m_val(0) {
	}
    Cmd3d (int cmd) : m_cmd(cmd), m_sub_cmd(0), m_src(0), m_parm(0), m_val(0) {
	}
	Cmd3d (int cmd, int sub_cmd) : m_cmd(cmd), m_sub_cmd(sub_cmd), m_src(0), m_parm(0), m_val(0) {
	}
	Cmd3d (int cmd, int sub_cmd, int src) : m_cmd(cmd), m_sub_cmd(sub_cmd), m_src(src), 
												m_parm(0), m_val(0) {
	}
	Cmd3d (int cmd, int sub_cmd, int src, int parm) : m_cmd(cmd), m_sub_cmd(sub_cmd), m_src(src), 
														m_parm(parm), m_val(0) {
	}
	Cmd3d (int cmd, int sub_cmd, int src, int parm, float val) : m_cmd(cmd), m_sub_cmd(sub_cmd), m_src(src), 
														m_parm(parm), m_val(val) {
	}
	virtual ~Cmd3d () {};
	void clr() { 
		m_cmd = 0;
		m_sub_cmd = 0;
		m_src = 0;
		m_parm = 0;
		m_val = 0;
	};
	
	int m_cmd;		// main cmd
	int m_sub_cmd;	// sub cmd
	int m_src;		// source of cmd/data
	int m_parm;		// int parm
	float m_val;	// float val

	friend std::ostream& operator<<(std::ostream& os, const Cmd3d &cmd) {
		os << boost::format("CMD %4d %4d %4d ") % cmd.m_cmd % cmd.m_sub_cmd % cmd.m_src;
		os << boost::format(" [%4d,%6.2f]") % cmd.m_parm % cmd.m_val;
		return os;
	};
	enum {
		CMD_NULL = 0,
		CMD_RUN,
		CMD_SETUP,
		CMD_CAL,
		CMD_TEST,
		CMD_SHOW,
		CMD_SEND_DATA,
		CMD_GET_DATA,
        CMD_LAST,
	};
	enum {
		SUB_CMD_NULL = 0,
		SUB_CMD_LAST,
	};
	enum {
		SRC_NULL = 0,
		SRC_LAST,
	};

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_cmd );
		ar & BOOST_SERIALIZATION_NVP( m_sub_cmd );
		ar & BOOST_SERIALIZATION_NVP( m_src );
		ar & BOOST_SERIALIZATION_NVP( m_parm );
		ar & BOOST_SERIALIZATION_NVP( m_val );
	}
};

typedef std::vector<Cmd3d> Cmd3d_v_t;
typedef std::deque<Cmd3d> Cmd3d_dq_t;
typedef std::map<int, Cmd3d> Cmd3d_m_t;

BOOST_CLASS_VERSION(Cmd3d, 0)

#endif
