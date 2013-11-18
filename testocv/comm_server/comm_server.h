#ifndef COMMGEOM3DSERVER_H
#define COMMGEOM3DSERVER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <list>
#include <vector>

#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/deque.hpp>

#include "Global.h"
#include "stock.hpp"


// Notification server - Manages multiple client connections
class comm_server
{
public:
	comm_server(int port = default_port_num) : m_Port(port), m_enable_flag(false),
										m_alg(0), m_DidExec(false) { m_name = default_name; };
	~comm_server() {};

	void StartServer(int port = 0);		// if non-zero port then use it
	int Notify(stock_v_t &stk_v);
	bool Enabled() { return(m_enable_flag); };
	bool Update(stock_dq_t &stk_dq);
	void Terminate();

protected:
	typedef s11n_example::connection_ptr SocketPtr;

	void StartAccept();
	void AcceptHandler(SocketPtr newConn, const boost::system::error_code& error);

	void WriteHandler(SocketPtr conn, const boost::system::error_code& error );
	void ReadHandler(SocketPtr conn, const boost::system::error_code& error );

	bool ValidateKey(SocketPtr sock);
	bool strFormat(stock &stk, std::string &str);

public:
	static const int default_port_num = 13110;
	static const std::string default_name;
	static const std::string default_service;
	static const std::string default_addr;
	static const std::string default_port;
	static const std::string xml_type;	// send xml object
	static const std::string str_type;	// send str using format

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

	stock_v_t m_recv_stk_v;		// recv data

	stock_dq_t m_access_stk_dq;		// dq of recv data, not retrieved

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

BOOST_CLASS_VERSION(comm_server, 2)

#endif // COMMGEOM3DSERVER_H