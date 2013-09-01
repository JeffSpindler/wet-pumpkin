//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <conio.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>

#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>

#include "comm_server.h"

bool get_input ( char *buffer, std::size_t size )
{
	int timeout = 1;
	std::time_t start = std::time ( 0 );
	std::size_t n = 0;
	while(1) {
		if ( n == 0 && std::difftime ( std::time ( 0 ), start ) >= timeout )
			return false;
		if ( _kbhit() ) {
			if ( n == size - 1 )
				break;
			char ch = (int)_getche();
			if ( ch == '\r' || ch == '/n' ) {
				break;
			} else {
				buffer[n++] = ch;
			}
		}
	}
	buffer[n] = '\0';
	return true;
}

void make_str(str_v_t &send_str_v)
{
	std::string test_str = "HEY send it will ya.";
	send_str_v.clear();
	send_str_v.push_back(test_str);
	test_str = "my oh my";
	send_str_v.push_back(test_str);
	test_str = "last try";
	send_str_v.push_back(test_str);
}

void make_stocks(stock_v_t &stock_v)
{
	stock stk;
	stk.code = "code1";
	stk.name = "s0";
	stock_v.push_back(stk);
	stk.name = "s1";
	stock_v.push_back(stk);
	stk.name = "s2";
	stock_v.push_back(stk);
}

int main(int argc, char* argv[])
{
	char inchar[1024];
	int bufsz = 1024;
	bool got_char_flag = false;

	stock_dq_t recv_dq;
	stock_v_t send_v;
	int send_count = 0;

	std::string cmdstr, argstr;

	comm_server m_server;
	m_server.save(std::string("wm_server.xml"));
	m_server.StartServer();	// startup everything

	while(cmdstr != "Q") {
		recv_dq.clear();
		if(m_server.Update(recv_dq)) {
			std::cout << "Recv size " << recv_dq.size() << std::endl;
			BOOST_FOREACH(stock stk, recv_dq) std::cout << stk << std::endl;
			std::cout << std::endl << std::endl;
			// send it back
			if(true) {
				send_v.clear();
				BOOST_FOREACH(stock stk, recv_dq) send_v.push_back(stk);
				m_server.Notify(send_v);
				// print it
				std::cout << "send back size " << send_v.size() << std::endl;
				BOOST_FOREACH(stock stk, send_v) std::cout << stk << std::endl;
				std::cout << "\n\n";
			}
		}

		if(cmdstr == "SEND") {
			std::cout << std::endl << "server send  ";
			boost::to_upper(argstr);
			send_count++;
			send_v.clear();

			make_stocks(send_v);

			m_server.Notify(send_v);

			std::cout << "send size " << send_v.size() << std::endl;
			BOOST_FOREACH(stock stk, send_v) std::cout << stk << std::endl;
			std::cout << std::endl << "sent " << "\n\n";
		}

		if ( !get_input ( inchar, bufsz) ) {
			cmdstr = "";
			continue;
		} else {
			std::cout << std::endl;
			std::cout << "> ";
		}
		cmdstr = inchar;
		int spaceLoc = cmdstr.find(' ');
		if(spaceLoc != -1)
		{
			argstr = cmdstr.substr(spaceLoc+1);
			cmdstr = cmdstr.substr(0, spaceLoc);
		}
		boost::to_upper(cmdstr);
	}
	m_server.Terminate();
	return 0;
}
