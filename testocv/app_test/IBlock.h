#ifndef IBLOCK_H
#define IBLOCK_H

#include <string>
#include <vector>
#include <map>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

class IGlobalAccess;
class IBlock;
typedef std::map<std::string,IBlock*> block_map_t;
typedef std::vector<IBlock*> block_v_t;
	
// Interface class for logic block

class IBlock
{
public:
	IBlock() {};
	virtual ~IBlock() {};

	bool setup(IGlobalAccess *data_access) { return(doSetup(data_access)); };

	const std::string Name() const {
		return const_cast<IBlock*>(this)->getName(); 
	}
	std::string& Name() {
		return(getName());
	}
	bool run(IGlobalAccess *data_access = NULL) { return(doRun(data_access)); };
	void print() { doPrint(); };

protected:
	std::string m_name;

private:
	virtual const std::string getName() const { return(m_name); };
	virtual std::string &getName() { return(m_name); };
	virtual bool doSetup(IGlobalAccess *data_access) = 0;
	virtual bool doRun(IGlobalAccess *data_access = NULL) = 0;
	virtual void doPrint() = 0;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_name);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(IBlock);
BOOST_CLASS_VERSION(IBlock, 0)
#endif

