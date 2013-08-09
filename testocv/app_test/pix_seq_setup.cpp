#include <cstdio>
#include <iostream>
#include <fstream>

//#include "AltSeq.h"
//#include "DoASeq.h"

#include "AppDataOne.h"
//#include "DoABlock.h"
//#include "DoBBlock.h"

#include "PixPtData.h"

#include "GlobalAccess.h"

#include "app_test.h"

using namespace P3D;

bool setup_pix_seq_data(GlobalAccess *data_access)
{
	// make pix pt storage
	PixPtData data1;
	//data1.Name() = "data1";
	data1.m_parm1 = 53;
	data1.m_parm2 = 77;
	if(!data_access->addGlobal(data1.Name(), &data1)) printf("bad data1\n");

	AppDataOne data2;
	data2.m_str_stuff = "data2_name";
	data2.m_value_int = 88;

	if(!data_access->addGlobal("data2", &data2)) printf("bad data2\n");

	return(true);
}

// create a test pix_seq
void create_pix_seq()
{
	// data needed -- add for globalaccess
	// Cams
	
	// image
	
	// rle

	// fts



	// get an image

	// thresh and rle

	// cca

	// select best

	// make into camrays

}

