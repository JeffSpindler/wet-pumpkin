#include <cstdio>
#include <iostream>


#include "AppDataOne.h"

#include "GlobalAccess.h"

#include "app_test.h"



int main( void )
{
	std::cout << "app_test\n\n" << std::endl;

	GlobalAccess data_access;

	data_access.init();
	AppDataOne data1;
	//data1.Name() = "DATA1";
	data1.m_str_stuff = "data1_name";
	data1.m_value_int = 53;

	if(!data_access.addGlobal("data1", &data1)) printf("bad data1\n");

	IGlobal *result;
	result = data_access.getGlobal("data1");
	if(result != NULL)
		result->print();
	

	std::cout << std::endl << std::endl;
	while(1);
	
}