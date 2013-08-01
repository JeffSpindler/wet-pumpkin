#include <cstdio>
#include <iostream>

#include "IBlock.h"

#include "ISeq.h"

#include "AppDataOne.h"
#include "DoABlock.h"

#include "GlobalAccess.h"

#include "app_test.h"



int main( void )
{
	std::cout << "app_test\n\n" << std::endl;

	//ISeq pix_seq;
	GlobalAccess data_access;

	data_access.init();
	AppDataOne data1;
	data1.m_str_stuff = "data1_name";
	data1.m_value_int = 53;
	if(!data_access.addGlobal("data1", &data1)) printf("bad data1\n");

	AppDataOne data2;
	data2.m_str_stuff = "data2_name";
	data2.m_value_int = 88;

	if(!data_access.addGlobal("data2", &data2)) printf("bad data2\n");

	IGlobal *result;
	result = data_access.getGlobal("data1");
	if(result != NULL)
		result->print();
	result = data_access.getGlobal("data2");
	if(result != NULL)
		result->print();

	DoABlock *block1 = new DoABlock;

	block1->init(&data_access);
	block1->run(&data_access);
	
	std::cout << std::endl << std::endl;
	while(1);
	
}