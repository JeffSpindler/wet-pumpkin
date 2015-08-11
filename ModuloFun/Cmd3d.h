// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/11/18)
//
// Cmd Object
//

#ifndef CMD3D_H
#define CMD3D_H

#include "Global.h"

class Cmd3d : public std::string
{
public:
    // This has an int cmd, sub_cmd
	// paired with int parm, int src, float val
	//
    // Construction and destruction.
    Cmd3d () { 	};
    Cmd3d (std::string cmd) : std::string(cmd) { };
    Cmd3d (std::string cmd, std::string resp) : std::string(cmd + " " + resp) { };


	//virtual ~Cmd3d () {};
	//void clr() { 
	//	m_cmd.clear();
	//	m_resp.clear();
	//	m_parm_dq.clear();
	//};
	
	//std::string m_cmd;		// cmd
	//std::string m_resp;		// resp
	//str_dq_t m_parm_dq;		// parameters

	//friend std::ostream& operator<<(std::ostream& os, const Cmd3d &cmd) {
	//	os << "CMD " << cmd << std::endl;
	//	//if(cmd.m_parm_dq.empty()) return os;
	//	//os << "  RESP ";
	//	//BOOST_FOREACH(std::string str, cmd.m_parm_dq) {
	//	//	os << " " << str;
	//	//}
	//	//os << std::endl;
	//	return os;
	//};
	const static std::string app;
	const static std::string arm;
	const static std::string set;
	const static std::string get;
	const static std::string debug;
	const static std::string sim;
	const static std::string log;
	const static std::string save;
	const static std::string load;
	const static std::string none;
	const static std::string status;
	const static std::string test;
	const static std::string file;
	const static std::string diag;

	const static std::string on;
	const static std::string off;
	const static std::string err;
	const static std::string ok;
	const static std::string all;

	const static std::string on_resp;
	const static std::string off_resp;
	const static std::string err_resp;
	const static std::string ok_resp;
	const static std::string none_resp;
	const static std::string clr;

	const static std::string version;
	const static std::string track;
	const static std::string elog;
	const static std::string lwip;
	const static std::string epl;
	const static std::string im;
	const static std::string live;
	const static std::string ball;
	const static std::string que;
	const static std::string id;
	const static std::string numcam;
	const static std::string shape;
	const static std::string im_parm;
	const static std::string reset;

	const static std::string file_id;
	const static std::string file_sensor;
	const static std::string file_app_org;
	const static std::string file_app_def;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(std::string);

		//ar & BOOST_SERIALIZATION_NVP(m_cmd);
		//ar & BOOST_SERIALIZATION_NVP(m_parm_dq);	
	}	
};

typedef std::vector<Cmd3d> Cmd3d_v_t;
typedef std::deque<Cmd3d> Cmd3d_dq_t;
typedef std::map<int, Cmd3d> Cmd3d_m_t;

BOOST_CLASS_VERSION(Cmd3d, 0)

#endif
