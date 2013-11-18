#ifndef COMM_CLIENT_H
#define COMM_CLIENT_H

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
class comm_client
{
public:
	comm_client(std::string name_str = default_name) {	m_name = default_name;
															m_enable_flag = false;
															m_alg = 0;
															m_DidExec = false; 
															m_port_str = default_port;
															m_host_str = default_addr;
	};
	comm_client(std::string host_str, std::string port_str) : 
											m_host_str(host_str), m_port_str(port_str) { 
															m_enable_flag = true;
															m_alg = 0;
															m_DidExec = false; };
	~comm_client() { m_DidExec = false; };

	void ClientLoop();
	void StartClient();		// use current setup
	void StartClient(std::string &host_addr, std::string port);
	void Terminate();
	bool Enabled() { return(m_enable_flag); };
	bool Update(stock_dq_t &stock_dq);
	int Notify(stock_v_t &stock_v);

public:
	static const std::string default_name;
	static const std::string default_service;
	static const std::string default_addr;
	static const std::string default_port;
	static const std::string xml_type;	// send xml object
	static const std::string str_type;	// send str using format

protected:
	typedef s11n_example::connection_ptr SocketPtr;

	void ConnectHandler(SocketPtr newConn, 
								   boost::asio::ip::tcp::resolver::iterator endpoint_iterator, 
								   const boost::system::error_code& error);
	void WriteHandler(SocketPtr conn, const boost::system::error_code& error );
	void ReadHandler(SocketPtr conn, const boost::system::error_code& error );

	bool ValidateKey(SocketPtr sock);
	bool strFormat(std::string &str, stock &stk);


protected:
	// setup vars
	std::string m_name;
	bool m_enable_flag;
	int m_alg;
	std::string m_service_str;
	std::string m_host_str;
	std::string m_port_str;
	std::string m_format_type;		// xml/str ...
	std::string m_format_str;		// boost format string for str output

	std::string m_spare_str;

	// internal vars
	boost::asio::io_service m_IOService;
	boost::asio::ip::tcp::endpoint m_Endpoint;
	boost::shared_ptr<boost::thread> m_ProcThread;
	boost::mutex m_Mutex;
	boost::mutex m_UpdateMut;

	std::list<SocketPtr> m_listSockets;

	stock_v_t m_recv_stock_v;		// vector of recv data

	stock_dq_t m_access_stock_dq;	// dq of recv data, not retrieved

	bool m_ClientLoopRun;
	bool m_DidExec;

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
		ar & BOOST_SERIALIZATION_NVP(m_host_str);
		ar & BOOST_SERIALIZATION_NVP(m_port_str);
		if(version > 1) {
			ar & BOOST_SERIALIZATION_NVP(m_format_type);
			ar & BOOST_SERIALIZATION_NVP(m_format_str);
		}
		ar & BOOST_SERIALIZATION_NVP(m_spare_str);
	}

};

BOOST_CLASS_VERSION(comm_client, 2)

#endif // COMMGEOM3DCLIENT_H