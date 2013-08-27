#ifndef IGLOBAL_H
#define IGLOBAL_H

#include <string>
#include <map>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>

class IGlobal;
typedef std::map<std::string,IGlobal*> global_map_t;
	
// Interface class for global managed data


class IGlobal
{
public:
	IGlobal() {};
	virtual ~IGlobal() {};

	void setup() { doSetup(); };

	const std::string& Name() const {
		return(getName());
	}
	void print() { doPrint(); };

protected:
	std::string m_name;
private:
	virtual const std::string &getName() const { return(m_name); };
	virtual void doSetup() = 0;
	virtual void doPrint() = 0;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_name);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(IGlobal);
BOOST_CLASS_VERSION(IGlobal, 0)

#endif

