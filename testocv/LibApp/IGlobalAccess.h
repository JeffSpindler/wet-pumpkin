#ifndef IGLOBALACCESS_H
#define IGLOBALACCESS_H

#include <string>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "IGlobal.h"

// Accessor Interface class for global managed data

class IGlobalAccess
{
public:
	IGlobalAccess() {};
	virtual ~IGlobalAccess() {};

	void setup() { doSetup(); };

	bool addGlobal(const std::string &name, IGlobal *data) {
		return(doAddGlobal(name, data));
	}
	IGlobal *removeGlobal(const std::string &name) {
		return(doRemoveGlobal(name));
	}
	IGlobal *getGlobal(const std::string &name) {
		return(doGetGlobal(name));
	}

private:
	virtual bool doSetup() = 0;
	virtual bool doAddGlobal(const std::string &name, IGlobal *data) = 0;
	virtual IGlobal *doRemoveGlobal(const std::string &name) = 0;
	virtual IGlobal *doGetGlobal(const std::string &name) = 0;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(IGlobalAccess);
BOOST_CLASS_VERSION(IGlobalAccess, 0)

#endif

