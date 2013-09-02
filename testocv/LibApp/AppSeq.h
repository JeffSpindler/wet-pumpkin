// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef APPSEQ_H
#define APPSEQ_H


#include "IBlock.h"
#include "ISeq.h"
#include "IGlobalAccess.h"

// Interface class for logic block

class AppSeq : public ISeq
{
public:
	AppSeq() : m_sim_flag(false) {
	};
	virtual ~AppSeq() {
		// must delete blocks
		for(unsigned i=0;i<m_seq.size();i++) {
			delete m_seq[i];
		}
		m_seq.clear();
	};

protected:  // local data
	bool m_sim_flag;
	
	block_v_t m_seq;	// main sequence

public:
	bool doSetup(IGlobalAccess *data_access) {  // Init all blocks
		std::cout << m_name << " doInit\n";
		bool all_OK_flag = true;	// if any are false, return false
		for(unsigned i=0;i<m_seq.size();i++) {
			if(!m_seq[i]->setup(data_access))
				all_OK_flag = false;
		}
		return(all_OK_flag);
	}

	bool doRun(IGlobalAccess *data_access = NULL) {  // stop on failure
		std::cout << m_name << " doRun\n";
		for(unsigned i=0;i<m_seq.size();i++) {
			if(!m_seq[i]->run(data_access)) {
				return(false);
			}
		}
		return(true);
	}

	bool doStep(IBlock *block = NULL, IGlobalAccess *data_access = NULL ) {
		if(block == NULL) {
			// step through all blocks
			std::cout << m_name << " doStep ALL\n";
			for(unsigned i=0;i<m_seq.size();i++) {
				if(!m_seq[i]->run(data_access)) {
					std::cout << m_name << " Failed\n";
					m_seq[i]->print();
					return(false);
				}
					m_seq[i]->print();			
			}
			return(true);
		}
		std::cout << m_name << " doStep\n";
		block->run(data_access);
		block->print();
		return(true);
	}

	// insert new element at end or before name indicated
	bool doInsertBlock(IBlock *block, const std::string &name_before = "") {
		// 1st check for duplicate name -- return false and do nothing if so
		block_v_t::iterator seq_i = m_seq.begin();
		for(unsigned i=0;i<m_seq.size();i++) {
			// find name -- if found add new block after
			if((*seq_i)->Name() == block->Name()) {
				return(false);
			}
			seq_i++;
		}
		if(name_before == "") {
			m_seq.push_back(block);
		} else {
			seq_i = m_seq.begin();
			for(unsigned i=0;i<m_seq.size();i++) {
				// find name -- if found add new block after
				if((*seq_i)->Name() == name_before) {
					m_seq.insert(seq_i, block);
					return(true);
				}
				seq_i++;
			}
		}
		return(true);
	}

	// start at beginning and remove 1st occurance
	bool doRemoveBlock(const std::string &name_remove) {
		block_v_t::iterator seq_i = m_seq.begin();
		for(unsigned i=0;i<m_seq.size();i++) {
			// find name -- if found remove
			if((*seq_i)->Name() == name_remove) {
				delete *seq_i;
				m_seq.erase(seq_i);
				return(true);
			}
			seq_i++;
		}		
		return(false);
	}

	IBlock* doGetBlock(int idx) {
		if(idx >= (int) m_seq.size())
			return(static_cast<IBlock*>(0));
		return(m_seq[idx]);
	}

	IBlock* doGetBlock(const std::string &name) {
		block_v_t::iterator seq_i = m_seq.begin();
		for(unsigned i=0;i<m_seq.size();i++) {
			// find name -- if found remove
			if((*seq_i)->Name() == name) {
				return(*seq_i);
			}
			seq_i++;
		}		
		return(static_cast<IBlock*>(0));
	}

	IBlock* doGetFirstBlock() {
		return(doGetBlock(0));
	}

	void doPrint() {
		std::cout << m_name << " doPrint\n";
		for(unsigned i=0;i<m_seq.size();i++) {
			m_seq[i]->print();
		}
	}
public:
	static bool Read(std::string &file_name, AppSeq *seq)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "DoABlock Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*seq);
		} 
		catch(std::exception &e) {
			std::cout << "AppSeq Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(AppSeq *seq, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "DoABlock Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(seq);
		} 
		catch(std::exception &e) {
			std::cout << "AppSeq Write " << e.what() << '\n';
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
		ar & BOOST_SERIALIZATION_NVP(m_seq);
	}
};

BOOST_CLASS_VERSION(AppSeq, 0)

#endif

