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

#include "comm_server.h"

using boost::asio::ip::tcp;

const std::string comm_server::default_name = "comm_server";
const std::string comm_server::default_service = "default_service";
const std::string comm_server::default_addr = "127.0.0.1";
const std::string comm_server::default_port = "13110";


void comm_server::StartServer(int port_num)
{
	if(port_num != 0) {
		m_Port = port_num;
	}

	std::cerr << m_name << "  StartServer Port " << m_Port <<std::endl;
	// Create the acceptor
	m_Acceptor.reset(new tcp::acceptor(m_IOService, tcp::endpoint(tcp::v4(), m_Port)));
	// Set up the accept process as a separate thread
	m_AcceptThread.reset(new boost::thread(&comm_server::StartAccept, this));
	boost::this_thread::sleep(boost::posix_time::milliseconds(30));
}
void comm_server::Terminate()
{
	if(m_DidExec) {
		m_IOService.stop();
		m_DidExec = false;
		m_AcceptThread->detach();
	}
}

void comm_server::StartAccept()
{
	// Create socket for a new connection
	SocketPtr NewConn(new s11n_example::connection(m_IOService));

	m_DidExec = true;

	// Start an async listen/accept process
    m_Acceptor->async_accept(NewConn->socket(),
        boost::bind(&comm_server::AcceptHandler, this, NewConn,
          boost::asio::placeholders::error));

	// Run the IO service
	m_IOService.run();
}

int comm_server::Notify(stock_v_t &stk_v)
{
	boost::unique_lock<boost::mutex> lock(m_Mutex);

	std::list<SocketPtr> badSockets;
	int iCount = 0;
	for(std::list<SocketPtr>::iterator it=m_listSockets.begin(); it!=m_listSockets.end(); it++)
	{
		try {
			// Send the data
			  (*it)->async_write(stk_v,
									boost::bind(&comm_server::WriteHandler, this, (*it),
									  boost::asio::placeholders::error));
			iCount++;
		}
		catch(boost::system::system_error &) {
			// An exception means that the client closed the connection
			// Add this socket to the bad list to be removed outside of the for loop
			std::cout << m_name << " comm_server::Notify ---Socket Error\n";		
			badSockets.push_back(*it);
		}
		catch(...) {  // Handle all remaining exceptions
			std::cout << m_name << " comm_server::Notify ----System Error Write failed\n";
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

void comm_server::AcceptHandler(SocketPtr newConn, const boost::system::error_code& error)
{
    if (!error)
    {
		std::cout << "AcceptHandler ---- new connection\n";
		// Store the newly connected socket in our list of active sockets
		boost::unique_lock<boost::mutex> lock(m_Mutex);

		// only one connect at a time
		m_listSockets.push_back(newConn);

		newConn->async_read(m_recv_stk_v, boost::bind(&comm_server::ReadHandler, 
								this, newConn, boost::asio::placeholders::error));		
    }
	// Start a new listen/accept process
    StartAccept();
}

  /// Handle completion of a write operation.
void comm_server::WriteHandler(SocketPtr conn, const boost::system::error_code& error )
{
    // Null for now
}

// copy over all inputs
bool comm_server::Update(stock_dq_t &stk_dq)
{
	boost::unique_lock<boost::mutex> lock(m_UpdateMut);
	
	while(!m_access_stk_dq.empty()) {
		stk_dq.push_back(m_access_stk_dq.front());	// copy it
		m_access_stk_dq.pop_front();	// remove it
	}
	return (!stk_dq.empty());
}

void comm_server::ReadHandler(SocketPtr conn, const boost::system::error_code& e)
{
    if (!e) {
		boost::unique_lock<boost::mutex> lock(m_UpdateMut);
		// copy path segs out to our dq of path segs
		for (std::size_t i = 0; i < m_recv_stk_v.size(); ++i) {
			m_access_stk_dq.push_back(m_recv_stk_v[i]);
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
      conn->async_read(m_recv_stk_v,
          boost::bind(&comm_server::ReadHandler, this, conn,
            boost::asio::placeholders::error));
    }
}

// convert PdFt3d to string for sending
bool strFormat(stock &stk, std::string &str) 
{
	//str = (boost::format("ft3d %4d %4d %4d ") % ft3d.m_label % ft3d.m_state % ft3d.m_type).str();
	return(true);
}

