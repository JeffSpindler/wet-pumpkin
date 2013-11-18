#ifndef GLOBAL_H
#define GLOBAL_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "boost/format.hpp"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>

typedef std::vector<std::string> str_v_t;
typedef std::deque<std::string> str_dq_t;
	
// Interface class for logic block
// These names must be unique -- enfored by user

template <typename ArchObject>
bool writeXML(ArchObject &obj, std::string &filestr)
{
   try {
	std::ofstream ofs(filestr);
	if(!ofs.good()) {
		return(false);
	}
	boost::archive::xml_oarchive oa(ofs);
	oa << BOOST_SERIALIZATION_NVP(obj);
	return(true);
   }
   catch (const std::exception &ex) {
      std::cout << "writeXML " << filestr << "  " << ex.what() << std::endl;
	return(false);
   }
}

template <typename ArchObject>
bool readXML(std::string &filestr, ArchObject &obj)
{
   try {
	std::ifstream ifs(filestr);
	boost::archive::xml_iarchive ia(ifs);

	ia >> BOOST_SERIALIZATION_NVP(obj);
	return(true);
   }
   catch (const std::exception &ex) {
      std::cout << "readXML " << filestr << "  " << ex.what() << std::endl;
	return(false);
   }
}


#endif

