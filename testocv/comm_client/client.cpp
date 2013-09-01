//
// client.cpp
//
// Copyright (c) 2013 Perform3-D LLC
//
//

#include <conio.h>
#include <iostream>

#include "CommGeom3dClient.h"

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

void make_g3ds(int tag, Geom3d_v_t &g3d_v)
{
	Geom3d g3d(tag);

	g3d.m_idx = 0;
	g3d_v.push_back(g3d);
	g3d.m_idx++;
	//g3d.m_tag = ++tag;
	g3d_v.push_back(g3d);
	//g3d.m_tag = ++tag;
	g3d.m_idx++;
	g3d_v.push_back(g3d);
}

int main(int argc, char* argv[])
{
	char inchar[1024];
	int bufsz = 1024;
	bool got_char_flag = false;

	Geom3d_dq_t recv_dq;
	Geom3d_v_t send_v;
	int send_count = 0;

	std::string cmdstr, argstr;

	CommGeom3dClient m_client;
	m_client.save(std::string("wm_client.xml"));
	m_client.StartClient();	// startup everything


	while(cmdstr != "Q") {
		recv_dq.clear();
		if(m_client.Update(recv_dq)) {
			std::cout << "Recv size " << recv_dq.size() << std::endl;
			BOOST_FOREACH(Geom3d g3d, recv_dq) std::cout << g3d << std::endl;
			std::cout << std::endl << std::endl;
		}

		if(cmdstr == "SEND") {
			std::cout << std::endl << "client send  ";
			boost::to_upper(argstr);
			send_count++;
			send_v.clear();

			make_g3ds(send_count, send_v);

			m_client.Notify(send_v);

			std::cout << "send size " << send_v.size() << std::endl;
			BOOST_FOREACH(Geom3d g3d, send_v) std::cout << g3d << std::endl;
			std::cout << std::endl << "client str sent " << "\n\n";
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

	m_client.Terminate();

	return 0;
}
