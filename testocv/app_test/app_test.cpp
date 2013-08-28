#include <cstdio>
#include <iostream>
#include <fstream>

#include "AltSeq.h"
#include "DoASeq.h"

#include "AppDataOne.h"
#include "DoABlock.h"
#include "DoBBlock.h"

#include "PixPtData.h"

#include "GlobalAccess.h"

#include "app_test.h"



bool ReadBlockFile(std::string &file_name, DoABlock *block)
{
	// 
	try {
		std::ifstream ifs(file_name);

		boost::archive::xml_iarchive ia(ifs);

		ia >> BOOST_SERIALIZATION_NVP(*block);
	} 
	catch(std::exception &e) {
		std::cout << e.what() << '\n';
	}
	return(true);
}

bool WriteBlockFile(IBlock *block, std::string &file_name)
{
	try {
		std::ofstream ofs(file_name);

		boost::archive::xml_oarchive oa(ofs);

		oa << BOOST_SERIALIZATION_NVP(block);
	} 
	catch(std::exception &e) {
		std::cout << e.what() << '\n';
	}
	return(true);
}


int seq_test_main( void )
{
	std::string PixSeqFile = "pix_seq.xml";

	std::cout << "app_test\n\n" << std::endl;

	GlobalAccess data_access;

	data_access.setup();
	AppDataOne data1;
	//data1.Name() = "data1";
	data1.m_str_stuff = "data1_name";
	data1.m_value_int = 53;
	if(!data_access.addGlobal("data1", &data1)) printf("bad data1\n");

	AppDataOne data2;
	data2.m_str_stuff = "data2_name";
	data2.m_value_int = 88;

	if(!data_access.addGlobal("data2", &data2)) printf("bad data2\n");

	std::cout << "Save data1 back\n\n" << std::endl;
	data1.print();
	std::string DataSavFileStr = "data_test_sav.xml";
	std::string DataFileStr = "data_test.xml";

	data1.Write(&data1, DataSavFileStr);

	std::cout << "Read data3 back\n\n" << std::endl;
	AppDataOne data3;
	data3.Read(DataFileStr, &data3);
	data3.print();

	IGlobal *result;
	result = data_access.getGlobal("data1");
	if(result != NULL)
		result->print();
	result = data_access.getGlobal("data2");
	if(result != NULL)
		result->print();

	std::cout << "Read/Write Blocks\n\n" << std::endl;

	DoABlock *block1 = new DoABlock;
	block1->Name() = "B1";
	block1->print();

	std::string IBlockFileStr = "testblockXX.xml";
	WriteBlockFile(block1, IBlockFileStr); 

	DoABlock *block5 = new DoABlock;
	ReadBlockFile(IBlockFileStr, block5);
	block5->print();

	DoBBlock *block6 = new DoBBlock;
	block6->Name() = "BB77";
	block6->print();



	std::string BlockFileSavStr = "testblock1_sav.xml";
	std::string BlockFileStr = "testblock1.xml";

	block1->Write(block1, BlockFileSavStr);

	block1->Read(BlockFileStr, block1);
	block1->print();


	block1->setup(&data_access);
	block1->run(&data_access);
	block1->print();
	
	DoABlock *block2 = new DoABlock;
	block2->Name() = "B2";

	block2->setup(&data_access);
	block2->run(&data_access);
	block2->print();

	DoASeq pix_seq;

	pix_seq.insertBlock(block1);
	pix_seq.insertBlock(block2);

	pix_seq.setup(&data_access);
	pix_seq.print();


	std::cout << "\n Write Seq\n";

	std::string SeqFileSavStr = "testseq1_sav.xml";
	std::string SeqFileStr = "testseq1.xml";


	// change the data
	AppDataOne *d_ptr = static_cast<AppDataOne*>(data_access.getGlobal("data1"));
	if(d_ptr != NULL) d_ptr->m_value_int = 99;

	d_ptr = static_cast<AppDataOne*>(data_access.getGlobal("data2"));
	if(d_ptr != NULL) d_ptr->m_value_int = 277;

	// rerun the seq
	pix_seq.setup(&data_access);
	pix_seq.run(&data_access);
	pix_seq.print();

	// change the seq
	DoABlock *block3 = new DoABlock;
	block3->Name() = "B3";
	DoABlock *block4 = new DoABlock;
	block4->Name() = "B4";

	block3->setup(&data_access);
	block3->run(&data_access);
	block3->print();

	std::cout << "\nAdd a Seq\n";
	pix_seq.insertBlock(block3);	// add at end
	pix_seq.setup(&data_access);
	pix_seq.run(&data_access);
	pix_seq.print();

	d_ptr = static_cast<AppDataOne*>(data_access.getGlobal("data2"));
	if(d_ptr != NULL) d_ptr->m_value_int = 999;
	d_ptr = static_cast<AppDataOne*>(data_access.getGlobal("data1"));
	if(d_ptr != NULL) d_ptr->m_value_int = 378;


	std::cout << "\nAdd after block 1\n";
	pix_seq.insertBlock(block4, "B2");	// add before block2
	pix_seq.setup(&data_access);
	pix_seq.run();
	pix_seq.print();

	pix_seq.insertBlock(block6, "B3");	// add before block2


	// remove a step and run
	std::cout << "\n remove the added step\n";
	pix_seq.removeBlock("B4");	// add before block2
	pix_seq.setup(&data_access);
	pix_seq.run();
	pix_seq.print();	

	// step some
	pix_seq.step(block2);

	pix_seq.step();
	
	DoASeq::Write(&pix_seq, SeqFileSavStr);

	DoASeq *rd_seq = new DoASeq;

	if(DoASeq::Read(SeqFileStr, rd_seq)) {
		rd_seq->print();
	}



	std::cout << std::endl << std::endl;
	while(1);
	
}

