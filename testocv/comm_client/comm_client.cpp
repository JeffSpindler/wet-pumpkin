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

#include "comm_client.h"

using boost::asio::ip::tcp;

const std::string comm_client::default_name = "comm_client";
const std::string comm_client::default_service = "default_service";
const std::string comm_client::default_addr = "127.0.0.1";
const std::string comm_client::default_port = "13110";


void comm_client::StartClient(std::string &host_addr, std::string port)
{
	m_host_str = host_addr;
	m_port_str = port;
	StartClient();
}

void comm_client::StartClient()
{
	m_enable_flag = true;
	std::cerr << "Start " << m_name << "Host " << m_host_str << "  Port " << m_port_str <<std::endl;
	m_ClientLoopRun = true;
	m_DidExec = true;
	m_ProcThread.reset(new boost::thread(&comm_client::ClientLoop, this));
	boost::this_thread::sleep(boost::posix_time::milliseconds(30));
}

void comm_client::Terminate()
{
	if(m_DidExec) {
		m_IOService.stop();
		m_DidExec = false;
		m_ProcThread->detach();
	}
}

void comm_client::ClientLoop()
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

			SocketPtr NewConn(new s11n_example::connection(m_IOService));

			NewConn->socket().async_connect(m_Endpoint,
												boost::bind(&comm_client::ConnectHandler, this, NewConn, 
												endpoint_iterator, boost::asio::placeholders::error));
			// Run the IO service
			m_IOService.run();
			// must have lost connection -- try again
			std::cerr << "Back from io service run, must reconnect\n" << std::endl;
			m_IOService.reset();
		}
	} catch (std::exception& e) {
		std::cerr << "ERROR while no io service run\n" << e.what() << std::endl;
		m_DidExec = false;
	}
}

// copy over all inputs
bool comm_client::Update(stock_dq_t &stock_dq)
{
	boost::unique_lock<boost::mutex> lock(m_UpdateMut);
	
	while(!m_access_stock_dq.empty()) {
		stock_dq.push_back(m_access_stock_dq.front());	// copy it
		m_access_stock_dq.pop_front();	// remove it
	}

	return (!stock_dq.empty());
}

int comm_client::Notify(stock_v_t &stock_v)
{
	boost::unique_lock<boost::mutex> lock(m_Mutex);

	std::list<SocketPtr> badSockets;
	int iCount = 0;
	for(std::list<SocketPtr>::iterator it=m_listSockets.begin(); it!=m_listSockets.end(); it++)
	{
		try {
			// Send the packet
			  (*it)->async_write(stock_v,
									boost::bind(&comm_client::WriteHandler, this, (*it),
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
			std::cout << "comm_client::Notify ----System Error Write failed\n";
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
void comm_client::ConnectHandler(SocketPtr newConn, 
								   boost::asio::ip::tcp::resolver::iterator endpoint_iterator, 
								   const boost::system::error_code& e)
{
    if (!e)
    {
		boost::unique_lock<boost::mutex> lock(m_Mutex);
		// only one connect at a time
		m_listSockets.clear();
		
		m_listSockets.push_back(newConn);

      // Successfully established connection. Start operation to read the list
      // of stocks. The connection::async_read() function will automatically
      // decode the data that is read from the underlying socket.
      newConn->async_read(m_recv_stock_v,
          boost::bind(&comm_client::ReadHandler, this, newConn, boost::asio::placeholders::error));

	  std::cerr << "got new connection -- ready to read" << std::endl;
    }
   // else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
   // {
   //   // Try the next endpoint.
   //   newConn->socket().close();
   //   boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
   //   newConn->socket().async_connect(endpoint,
   //       boost::bind(&comm_client::ConnectHandler, this, newConn, 
   //         ++endpoint_iterator, boost::asio::placeholders::error));
	  //std::cerr << "try another endpoint -- trying to connect" << e.message() << std::endl;
   // }
    else
    {
      // An error occurred. Log it and return. Since we are not starting a new
      // operation the io_service will run out of work to do and the client will
      // exit.
      //std::cerr << "ConnectHandler error  -- try again\n" << e.message() << std::endl;
      newConn->socket().close();
      boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
      newConn->socket().async_connect(endpoint,
          boost::bind(&comm_client::ConnectHandler, this, newConn, 
            endpoint_iterator, boost::asio::placeholders::error));
	}
}

  /// Handle completion of a write operation.
void comm_client::WriteHandler(SocketPtr conn, const boost::system::error_code& error )
{
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
}


void comm_client::ReadHandler(SocketPtr conn, const boost::system::error_code& e)
{
    if (!e) {
		boost::unique_lock<boost::mutex> lock(m_UpdateMut);
		// copy path segs out to our dq of path segs
		for (std::size_t i = 0; i < m_recv_stock_v.size(); ++i) {
			m_access_stock_dq.push_back(m_recv_stock_v[i]);
			//m_path_seg_v[i].print(1);
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
		conn->async_read(m_recv_stock_v,
							boost::bind(&comm_client::ReadHandler, this, conn,
							boost::asio::placeholders::error));
    }
}

