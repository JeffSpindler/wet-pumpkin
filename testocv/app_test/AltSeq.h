#ifndef ALTSEQ_H
#define ALTSEQ_H


#include "IBlock.h"
#include "ISeq.h"
#include "IGlobalAccess.h"

// Interface class for logic block

class AltSeq : public ISeq
{
public:
	AltSeq() :	m_sim_flag(false),
				m_int_var(0) {
	};
	virtual ~AltSeq() {
	};

protected:  // local data
	bool m_sim_flag;
	int m_int_var;

	
public:
	bool doSetup(IGlobalAccess *data_access) {  // Init all blocks
		return(true);
	}

	bool doRun(IGlobalAccess *data_access = NULL) {  // stop on failure
		return(true);
	}

	bool doStep(IBlock *block = NULL, IGlobalAccess *data_access = NULL ) {
		return(true);
	}

	// insert new element at end or before name indicated
	bool doInsertBlock(IBlock *block, const std::string &name_before = "") {
		return(true);
	}

	// start at beginning and remove 1st occurance
	bool doRemoveBlock(const std::string &name_remove) {
		return(true);
	}

	IBlock* doGetBlock(int idx) {
		return(static_cast<IBlock*>(0));
	}

	IBlock* doGetBlock(const std::string &name) {		
		return(static_cast<IBlock*>(0));
	}

	IBlock* doGetFirstBlock() {
		return(doGetBlock(0));
	}

	void doPrint() {
	}
public:
	static bool Read(std::string &file_name, AltSeq *data_ptr)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "AltSeq Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AltSeq Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(AltSeq *data_ptr, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "AltSeq Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(data_ptr);
		} 
		catch(std::exception &e) {
			std::cout << "AltSeq Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ISeq);
		ar & BOOST_SERIALIZATION_NVP(m_sim_flag);
		ar & BOOST_SERIALIZATION_NVP(m_int_var);
	}
};

BOOST_CLASS_VERSION(AltSeq, 0)
BOOST_CLASS_EXPORT(AltSeq);
#endif

