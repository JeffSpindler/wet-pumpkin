// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/08/29)
//
// this sends data to/from a server
//
// data is sent with an 8 byte hex header which leads.
//
// This is derived from the boost asio serialization example server side using boost threads.
// 

#include <boost/bind.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "CommGeom3dServer.h"

using boost::asio::ip::tcp;

const std::string CommGeom3dServer::default_name = "CommGeom3dServer";
const std::string CommGeom3dServer::default_service = "default_service";
const std::string CommGeom3dServer::default_addr = "127.0.0.1";
const std::string CommGeom3dServer::default_port = "13110";


void CommGeom3dServer::StartServer(int port_num)
{
	if(port_num != 0) {
		m_Port = port_num;
	}

	std::cerr << m_name << "  StartServer Port " << m_Port <<std::endl;
	// Create the acceptor
	m_Acceptor.reset(new tcp::acceptor(m_IOService, tcp::endpoint(tcp::v4(), m_Port)));
	// Set up the accept process as a separate thread
	m_AcceptThread.reset(new boost::thread(&CommGeom3dServer::StartAccept, this));
	boost::this_thread::sleep(boost::posix_time::milliseconds(30));
}
void CommGeom3dServer::Terminate()
{
	if(m_DidExec) {
		m_IOService.stop();
		m_DidExec = false;
		m_AcceptThread->detach();
	}
}

void CommGeom3dServer::StartAccept()
{
	// Create socket for a new connection
	SocketPtr NewConn(new serialize_socket(m_IOService));

	m_DidExec = true;

	// Start an async listen/accept process
    m_Acceptor->async_accept(NewConn->socket(),
        boost::bind(&CommGeom3dServer::AcceptHandler, this, NewConn,
          boost::asio::placeholders::error));

	// Run the IO service
	m_IOService.run();
}

int CommGeom3dServer::Notify(Geom3d_v_t &g3d_v)
{
	boost::unique_lock<boost::mutex> lock(m_Mutex);

	std::list<SocketPtr> badSockets;
	int iCount = 0;
	for(std::list<SocketPtr>::iterator it=m_listSockets.begin(); it!=m_listSockets.end(); it++)
	{
		try {
			// Send the data
			  (*it)->async_write(g3d_v,
									boost::bind(&CommGeom3dServer::WriteHandler, this, (*it),
									  boost::asio::placeholders::error));
			iCount++;
		}
		catch(boost::system::system_error &) {
			// An exception means that the client closed the connection
			// Add this socket to the bad list to be removed outside of the for loop
			std::cout << m_name << " CommGeom3dServer::Notify ---Socket Error\n";		
			badSockets.push_back(*it);
		}
		catch(...) {  // Handle all remaining exceptions
			std::cout << m_name << " CommGeom3dServer::Notify ----System Error Write failed\n";
		}
	}

	// Remove all sockets
	for(std::list<SocketPtr>::iterator it=badSockets.begin(); it!=badSockets.end(); it++)
	{
		std::cout << "----Remove dead sockets\n";
		m_listSockets.remove(*it);
	}
	return iCount;
}

void CommGeom3dServer::AcceptHandler(SocketPtr newConn, const boost::system::error_code& error)
{
    if (!error)
    {
		std::cout << "AcceptHandler ---- new connection\n";
		// Store the newly connected socket in our list of active sockets
		boost::unique_lock<boost::mutex> lock(m_Mutex);

		// only one connect at a time
		m_listSockets.push_back(newConn);

		newConn->async_read(m_recv_g3d_v, boost::bind(&CommGeom3dServer::ReadHandler, 
								this, newConn, boost::asio::placeholders::error));		
    }
	// Start a new listen/accept process
    StartAccept();
}

  /// Handle completion of a write operation.
void CommGeom3dServer::WriteHandler(SocketPtr conn, const boost::system::error_code& error )
{
    // Null for now
}

// copy over all inputs
bool CommGeom3dServer::Update(Geom3d_dq_t &g3d_dq)
{
	boost::unique_lock<boost::mutex> lock(m_UpdateMut);
	
	while(!m_access_g3d_dq.empty()) {
		g3d_dq.push_back(m_access_g3d_dq.front());	// copy it
		m_access_g3d_dq.pop_front();	// remove it
	}
	return (!g3d_dq.empty());
}

void CommGeom3dServer::ReadHandler(SocketPtr conn, const boost::system::error_code& e)
{
    if (!e) {
		boost::unique_lock<boost::mutex> lock(m_UpdateMut);
		// copy path segs out to our dq of path segs
		for (std::size_t i = 0; i < m_recv_g3d_v.size(); ++i) {
			m_access_g3d_dq.push_back(m_recv_g3d_v[i]);
			//m_path_seg_v[i].print(1);
		}
	} else {
		// An error occurred.
		std::cerr << m_name << " ReadHandler Error\n" << e.message() << std::endl;
	}

    if (!e)
    {
      // Successfully established connection. Start operation to read the data
      // The connection::async_read() function will automatically
      // decode the data that is read from the underlying socket.
      conn->async_read(m_recv_g3d_v,
          boost::bind(&CommGeom3dServer::ReadHandler, this, conn,
            boost::asio::placeholders::error));
    }
}
