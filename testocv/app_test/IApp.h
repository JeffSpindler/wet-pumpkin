#ifndef IAPP_H
#define IAPP_H

#include <string>


#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/utility.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

class IGlobalAccess;
class IBlock;
typedef std::map<std::string,IBlock*> block_map_t;
typedef std::vector<IBlock*> block_v_t;
	
// Interface class for logic block
// These names must be unique -- enfored by user

class ISeq   // : boost::noncopyable
{
public:
	ISeq() {};
	virtual ~ISeq() {};

	bool setup(IGlobalAccess *data_access) { return(doSetup(data_access)); };
	bool run(IGlobalAccess *data_access = NULL) { return(doRun(data_access)); };

	bool step(IBlock *block = NULL, IGlobalAccess *data_access = NULL ) {
		return(doStep(block, data_access));
	}

	bool insertBlock(IBlock *block, const std::string &name_before = "") {
		return(doInsertBlock(block, name_before));
	}
	bool removeBlock(const std::string &name_remove) {
		return(doRemoveBlock(name_remove));
	}
	IBlock* getBlock(int idx) {
		return(doGetBlock(idx));
	}
	IBlock* getBlock(const std::string &name) {
		return(doGetBlock(name));
	}
	IBlock* getFirstBlock() {
		return(doGetFirstBlock());
	}

	std::string& Name() {
		return(getName());
	}

	void print() { doPrint(); };

protected:
	std::string m_name;

private:
	virtual bool doSetup(IGlobalAccess *data_access) = 0;
	virtual bool doRun(IGlobalAccess *data_access = NULL) = 0;
	virtual bool doStep(IBlock *block = NULL, IGlobalAccess *data_access = NULL) = 0;

	virtual bool doInsertBlock(IBlock *block, const std::string &name_before = "") = 0;
	virtual bool doRemoveBlock(const std::string &name_remove) = 0;
	virtual IBlock* doGetBlock(int idx) = 0;
	virtual IBlock* doGetBlock(const std::string &name) = 0;
	virtual IBlock* doGetFirstBlock() = 0;
	virtual std::string &getName() { return(m_name); };

	virtual void doPrint() = 0;

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_name);
	}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(ISeq);
BOOST_CLASS_VERSION(ISeq, 0)

#endif

