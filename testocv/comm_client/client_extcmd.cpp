//
// client.cpp
//
// Copyright (c) 2013 Perform3-D LLC
//
//

#include <conio.h>
#include <iostream>

#include "CommExtCmdClient.h"

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

void make_extcmds(int tag, ExtCmd_v_t &extcmd_v)
{
	ExtCmd extcmd;
	Cmd3d cmd(1,2,3,4,5.0);
	extcmd.m_cmd_v.push_back(cmd);

	Geom3d g3d(tag);

	g3d.m_idx = 0;
	extcmd.m_g3d_v.push_back(g3d);
	g3d.m_idx++;
	//g3d.m_tag = ++tag;
	extcmd.m_g3d_v.push_back(g3d);
	//g3d.m_tag = ++tag;
	g3d.m_idx++;
	extcmd.m_g3d_v.push_back(g3d);
	extcmd_v.push_back(extcmd);
}

int main(int argc, char* argv[])
{
	char inchar[1024];
	int bufsz = 1024;
	bool got_char_flag = false;

	ExtCmd_dq_t recv_dq;
	ExtCmd_v_t send_v;
	int send_count = 0;

	std::string cmdstr, argstr;

	CommExtCmdClient m_client;
	std::string filename = "test_client_extcmd.xml";
	if(!m_client.load(filename)) {
		m_client.save(filename);
	} else {
		std::cout << "loaded " << filename << "  client setup\n\n";
	}
	m_client.StartClient();	// startup everything


	while(cmdstr != "Q") {
		recv_dq.clear();
		if(m_client.Update(recv_dq)) {
			std::cout << "Recv size " << recv_dq.size() << std::endl;
			BOOST_FOREACH(ExtCmd cmd, recv_dq) std::cout << cmd << std::endl;
			std::cout << std::endl << std::endl;
		}

		if(cmdstr == "SEND") {
			std::cout << std::endl << "client send  ";
			boost::to_upper(argstr);
			send_count++;
			send_v.clear();

			make_extcmds(send_count, send_v);

			m_client.Notify(send_v);

			std::cout << "send size " << send_v.size() << std::endl;
			BOOST_FOREACH(ExtCmd cmd, send_v) std::cout << cmd << std::endl;
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
