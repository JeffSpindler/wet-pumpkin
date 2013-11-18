// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/29)
//
// this sends data to/from a client
//
// data is sent as XML with a 8 byte hex header which leads.
//
// This is derived from the boost asio serialization example client side using boost threads.
// 

#include <boost/bind.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "CommExtCmdClient.h"

using boost::asio::ip::tcp;

const std::string CommExtCmdClient::default_name = "CommExtCmdClient";
const std::string CommExtCmdClient::default_service = "default_service";
const std::string CommExtCmdClient::default_addr = "127.0.0.1";
const std::string CommExtCmdClient::default_port = "13110";
const std::string CommExtCmdClient::xml_type = "xml";		// serialize using xml
const std::string CommExtCmdClient::str_pt_type = "str_pt";		// str pt using format
const std::string CommExtCmdClient::str_ray_type = "str_ray";		// str camray using format
const std::string CommExtCmdClient::str_frame_type = "str_frame";		// str frame using format


void CommExtCmdClient::StartClient(std::string &host_addr, std::string port)
{
	m_host_str = host_addr;
	m_port_str = port;
	StartClient();
}

void CommExtCmdClient::StartClient()
{
	if(!m_enable_flag) return;
	std::cout << "Start " << m_name << "Host " << m_host_str << "  Port " << m_port_str <<std::endl;
	std::cout << m_format_type << " " << m_format_str << std::endl;;
	m_ClientLoopRun = true;
	m_DidExec = true;
	m_ProcThread.reset(new boost::thread(&CommExtCmdClient::ClientLoop, this));
	boost::this_thread::sleep(boost::posix_time::milliseconds(30));
}

void CommExtCmdClient::Terminate()
{
	if(m_DidExec) {
		m_ClientLoopRun = false;
		m_IOService.stop();
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		m_DidExec = false;
		m_ProcThread->detach();
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}

void CommExtCmdClient::ClientLoop()
{
	try {

		while(m_ClientLoopRun) {
			// Start an asynchronous connect operation.
			// Resolve the host name into an IP address.
			boost::asio::ip::tcp::resolver resolver(m_IOService);
			boost::asio::ip::tcp::resolver::query query(m_host_str, m_port_str);
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			m_Endpoint = *endpoint_iterator;

			std::cerr << "Start looking for new connection" << std::endl;

			SocketPtr NewConn(new serialize_socket(m_IOService));

			NewConn->socket().async_connect(m_Endpoint,
												boost::bind(&CommExtCmdClient::ConnectHandler, this, NewConn, 
												endpoint_iterator, boost::asio::placeholders::error));
			// Run the IO service
			if(!m_ClientLoopRun) break;
			m_IOService.run();
			// must have lost connection -- try again
			std::cerr << "Back from io service run, must reconnect\n" << std::endl;
			if(!m_ClientLoopRun) break;
			m_IOService.reset();
		}
	} catch (std::exception& e) {
		std::cerr << "ERROR while no io service run\n" << e.what() << std::endl;
		m_DidExec = false;
	}
}

// copy over all inputs
bool CommExtCmdClient::Update(ExtCmd_dq_t &g3d_dq)
{
	boost::unique_lock<boost::mutex> lock(m_UpdateMut);

	if(!m_enable_flag) return false;

	while(!m_access_g3d_dq.empty()) {
		g3d_dq.push_back(m_access_g3d_dq.front());	// copy it
		m_access_g3d_dq.pop_front();	// remove it
	}

	return (!g3d_dq.empty());
}

int CommExtCmdClient::Notify(ExtCmd_v_t &g3d_v)
{
	boost::unique_lock<boost::mutex> lock(m_Mutex);

	if(!m_enable_flag) return false;
	std::list<SocketPtr> badSockets;
	int iCount = 0;
	for(std::list<SocketPtr>::iterator it=m_listSockets.begin(); it!=m_listSockets.end(); it++)
	{
		try {
			// Send the packet
			  (*it)->async_write(g3d_v,
									boost::bind(&CommExtCmdClient::WriteHandler, this, (*it),
									  boost::asio::placeholders::error));

			iCount++;
		}
		catch(boost::system::system_error &) {
			// An exception means that the client closed the connection
			// Add this socket to the bad list to be removed outside of the for loop
			std::cout << "----Socket Error\n";
		
			badSockets.push_back(*it);
		}
		catch(...) {  // Handle all remaining exceptions
			std::cout << "CommExtCmdClient::Notify ----System Error Write failed\n";
		}
	}

	// Remove all sockets
	for(std::list<SocketPtr>::iterator it=badSockets.begin(); it!=badSockets.end(); it++)
	{
		std::cout << "----Removing dead conn\n";
		m_listSockets.remove(*it);
	}
	return iCount;
}

  /// Handle completion of a connect operation.
void CommExtCmdClient::ConnectHandler(SocketPtr newConn, 
								   boost::asio::ip::tcp::resolver::iterator endpoint_iterator, 
								   const boost::system::error_code& e)
{
    if (!e) {
		boost::unique_lock<boost::mutex> lock(m_Mutex);
		// only one connect at a time
		m_listSockets.clear();
		
		m_listSockets.push_back(newConn);

		// Successfully established connection. Start operation to read objects.
		// The connection::async_read() function will automatically
		// decode the data that is read from the underlying socket.
		if(m_format_type != xml_type) {
			newConn->async_read(m_recv_str,
									boost::bind(&CommExtCmdClient::ReadHandler, this, newConn, 
									boost::asio::placeholders::error));
		} else {
			newConn->async_read(m_recv_g3d_v,
									boost::bind(&CommExtCmdClient::ReadHandler, this, newConn, 
									boost::asio::placeholders::error));
		}

		std::cerr << "got new connection -- ready to read" << std::endl;
    } else {
      // An error occurred. Log it and return. Since we are not starting a new
      // operation the io_service will run out of work to do and the client will
      // exit.
      //std::cerr << "ConnectHandler error  -- try again\n" << e.message() << std::endl;
      newConn->socket().close();
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
      newConn->socket().async_connect(endpoint,
          boost::bind(&CommExtCmdClient::ConnectHandler, this, newConn, 
            endpoint_iterator, boost::asio::placeholders::error));
	}
}

  /// Handle completion of a write operation.
void CommExtCmdClient::WriteHandler(SocketPtr conn, const boost::system::error_code& error )
{
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
}

// upon a record read -- handle resulting data
void CommExtCmdClient::ReadHandler(SocketPtr conn, const boost::system::error_code& e)
{
	bool str_type_flag = m_format_type != xml_type;
    if (!e) {
		boost::unique_lock<boost::mutex> lock(m_UpdateMut);
		if(str_type_flag) {
			static ExtCmd g3d;
			if(strFormat(m_recv_str, g3d)) {
				if(false) std::cout << "recv " << g3d << std::endl;
				m_access_g3d_dq.push_back(g3d);
			}
		} else {
			// copy path segs out to our dq of path segs
			for (std::size_t i = 0; i < m_recv_g3d_v.size(); ++i) {
				m_access_g3d_dq.push_back(m_recv_g3d_v[i]);
				//m_path_seg_v[i].print(1);
			}
		}
		//std::cout << "recv " <<  m_path_seg_v.size() << " psegs " << std::endl;
	} else {
		// An error occurred.
		std::cerr << "ReadHandler Error\n" << e.message() << std::endl;
		//conn->socket().close();

		// Since we are not starting a new operation the io_service will run out of
		// work to do and the client will exit.	-- we could instead setup a new connection handler
	}

    if (!e)
    {
		// Successfully established connection. Start operation to read data. 
		//   The connection::async_read() function will automatically
		// decode the data that is read from the underlying socket.
		if(str_type_flag) {
			conn->async_read(m_recv_str,
								boost::bind(&CommExtCmdClient::ReadHandler, this, conn,
								boost::asio::placeholders::error));
		} else {
			conn->async_read(m_recv_g3d_v,
								boost::bind(&CommExtCmdClient::ReadHandler, this, conn,
								boost::asio::placeholders::error));
		}
    }
}


