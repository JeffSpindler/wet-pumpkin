#ifndef GLOBALACCESS_H
#define GLOBALACCESS_H

#include <map>
#include <string>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "IGlobalAccess.h"



// Accessor Interface class for global managed data

class GlobalAccess : public IGlobalAccess
{
public:
	GlobalAccess() {};
	virtual ~GlobalAccess() {};

protected:  // data storage
	global_map_t m_global_m;	// store by name

private:
	// only clears values, leaves names
	virtual bool doInit() {
		global_map_t::iterator i = m_global_m.begin();
		while(i != m_global_m.end()) {
			(*i).second->init();
			i++;
		}
		return(true);
	}

	virtual bool doAddGlobal(const std::string &name, IGlobal *value) {
		global_map_t::iterator i = m_global_m.find(name);
		if(i != m_global_m.end()) return(false);

		m_global_m.insert(std::make_pair(name, value));
		return(true);
	}
	virtual IGlobal *doRemoveGlobal(const std::string &name) {
		global_map_t::iterator i = m_global_m.find(name);
		if(i == m_global_m.end()) return(static_cast<IGlobal*> (0));

		m_global_m.erase(i);
		return(i->second);
	}
	virtual IGlobal *doGetGlobal(const std::string &name) {
		global_map_t::iterator i = m_global_m.find(name);
		if(i == m_global_m.end()) return(static_cast<IGlobal*> (0));

		return(i->second);
	}

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
	}
};


#endif

