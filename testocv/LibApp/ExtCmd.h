// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/11/18)
//
// External Cmd Object
//
// contains possibly empty vectors of cmd3d, im, geom3d to allow
// exchanging cmds and data of any type via a que
//
#ifndef EXTCMD_H
#define EXTCMD_H

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

#include "Cmd3d.h"
#include "Im.h"
#include "Geom3d.h"

class ExtCmd
{
public:
    // This has an int cmd, sub_cmd
	// paired with int parm, int src, float val
	//
    // Construction and destruction.
    ExtCmd () : m_tag(0) {
	};
    ExtCmd (int tag) : m_tag(tag) {
	};
	ExtCmd (Cmd3d &cmd) : m_tag(0) {
		m_cmd_v.push_back(cmd);
	};
	ExtCmd (Cmd3d &cmd, Im &im) : m_tag(0) {
		m_cmd_v.push_back(cmd);
		m_im_v.push_back(im);
	};
	ExtCmd (Cmd3d &cmd, Geom3d &g3d) : m_tag(0) {
		m_cmd_v.push_back(cmd);
		m_g3d_v.push_back(g3d);
	};

	virtual ~ExtCmd () {};
	void clr() { 
		m_cmd_v.clear();
		m_im_v.clear();
		m_g3d_v.clear();
	};
	
	int m_tag;				// id/mode value
	Cmd3d_v_t m_cmd_v;		// cmds
	Im_v_t m_im_v;			// ims 
	Geom3d_v_t m_g3d_v;		// geom3ds

	friend std::ostream& operator<<(std::ostream& os, const ExtCmd &cmd) {
		os << cmd.m_tag << "  ";
		if(cmd.m_cmd_v.size()) os << cmd.m_cmd_v.front() << std::endl;
		if(cmd.m_im_v.size()) os << cmd.m_im_v.front() << std::endl;
		if(cmd.m_g3d_v.size()) os << cmd.m_g3d_v.front() << std::endl;
		return os;
	};
	enum {
		TAG_NULL = 0,
	};

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( m_tag );
		ar & BOOST_SERIALIZATION_NVP( m_cmd_v );
		ar & BOOST_SERIALIZATION_NVP( m_im_v );
		ar & BOOST_SERIALIZATION_NVP( m_g3d_v );
	}
};

typedef std::vector<ExtCmd> ExtCmd_v_t;
typedef std::deque<ExtCmd> ExtCmd_dq_t;
typedef std::map<int, ExtCmd> ExtCmd_m_t;

BOOST_CLASS_VERSION(ExtCmd, 0)

#endif
