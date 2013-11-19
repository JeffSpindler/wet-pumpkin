#ifndef COMMEXTCMDSERVER_H
#define COMMEXTCMDSERVER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <list>
#include <vector>

#include "serialize_socket.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/deque.hpp>

#include "Global.h"
#include "ExtCmd.h"


// Notification server - Manages multiple client connections
class CommExtCmdServer
{
public:
	CommExtCmdServer(int port = default_port_num) : m_Port(port), m_enable_flag(false),
										m_format_type(xml_type),
										m_alg(0), m_DidExec(false) { m_name = default_name; };
	~CommExtCmdServer() {};

	void StartServer(int port = 0);		// if non-zero port then use it
	int Notify(ExtCmd_v_t &extcmd_v);
	bool Enabled() { return(m_enable_flag); };
	bool Update(ExtCmd_dq_t &extcmd_dq);
	void Terminate();

protected:
	typedef serialize_socket_ptr SocketPtr;

	void StartAccept();
	void AcceptHandler(SocketPtr newConn, const boost::system::error_code& error);

	void WriteHandler(SocketPtr conn, const boost::system::error_code& error );
	void ReadHandler(SocketPtr conn, const boost::system::error_code& error );

	bool ValidateKey(SocketPtr sock);

public:
	static const int default_port_num = 13210;
	static const std::string default_name;
	static const std::string default_service;
	static const std::string default_addr;
	static const std::string default_port;
	static const std::string xml_type;	// send xml object
	static const std::string str_pt_type;	// send str using format
	static const std::string str_ray_type;	// send str using format
	static const std::string str_frame_type;	// send str using format

protected:
	// setup vars
	std::string m_name;
	bool m_enable_flag;
	int m_alg;
	std::string m_service_str;
	std::string m_client_str;
	int m_Port;
	std::string m_format_type;		// xml/str ...
	std::string m_format_str;		// boost format string for str output
	std::string m_spare_str;

	boost::asio::io_service m_IOService;
	boost::shared_ptr<boost::asio::ip::tcp::acceptor> m_Acceptor;
	boost::shared_ptr<boost::thread> m_AcceptThread;
	boost::mutex m_Mutex;
	boost::mutex m_UpdateMut;
	bool m_DidExec;

	std::list<SocketPtr> m_listSockets;

	ExtCmd_v_t m_recv_extcmd_v;		// recv data

	ExtCmd_dq_t m_access_extcmd_dq;		// dq of recv data, not retrieved

public:
	bool save(std::string &filename) 
			{ return(writeXML(*this, filename)); }
	bool load(std::string &filename) 
			{ return(readXML(filename, *this)); }

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_name);
		ar & BOOST_SERIALIZATION_NVP(m_enable_flag);
		ar & BOOST_SERIALIZATION_NVP(m_alg);
		ar & BOOST_SERIALIZATION_NVP(m_service_str);
		ar & BOOST_SERIALIZATION_NVP(m_client_str);
		ar & BOOST_SERIALIZATION_NVP(m_Port);
		if(version > 1) {
			ar & BOOST_SERIALIZATION_NVP(m_format_type);
			ar & BOOST_SERIALIZATION_NVP(m_format_str);
		}
		ar & BOOST_SERIALIZATION_NVP(m_spare_str);
	}
};

BOOST_CLASS_VERSION(CommExtCmdServer, 2)

#endif // COMMGEOM3DSERVER_H