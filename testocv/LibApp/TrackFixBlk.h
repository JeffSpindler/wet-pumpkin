// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#ifndef TRACKFIXBLK_H
#define TRACKFIXBLK_H

#include "Global.h"

#include "IBlock.h"

#include "CamCalData.h"
#include "PixPtData.h"
#include "Geom3dData.h"

#include "IGlobalAccess.h"

#include "Mat.h"
#include "Wm5GVector.h"
#include "Wm5GMatrix.h"


	
// Class for logic block

class TrackFixBlk : public IBlock
{
public:
	TrackFixBlk() :  m_cals(NULL), m_pts(NULL),
						m_fix(NULL), m_camrays(NULL), 
						m_model(NULL), m_cur(NULL), 
						m_val1(0), m_val2(0)  {
		m_name = "TrackFixBlk";
	};
	virtual ~TrackFixBlk() {};

	bool doSetup(IGlobalAccess *data_access);
	bool doRun(IGlobalAccess *data_access = NULL);
	void doPrint();

protected:
	Geom3dData *m_camrays;
	Geom3dData *m_fix;
	Geom3dData *m_model;
	Geom3dData *m_cur;

	CamCalData *m_cals;
	PixPtData *m_pts;

	int m_val1;
	float m_val2;

	// computed
	int m_num_mod_pts;		// pts in model
	Wm5::GVectord m_mod_com;
	Mat m_mod_pt_mat;
	Mat m_wt_mat;
	double wts_sum;
	Mat m_cur_pt_mat;
	std::map<int, Wm5::GVectord> m_mod_pt_map;	// map by tag of mod pts
	std::map<int, double> m_mod_wt_map;			// map by tag of mod wts
	Mat m_svd_U;		// U mat from SVD
	Mat m_svd_VT;		// V transpose mat from SVD
	Mat m_svd_Diag;	// Diag mat
	Mat m_Diag_mat;		// Diag result mat
	Mat m_covar_mat;	// covariant matrix to solve
	Wm5::GVectord m_cur_com;
	Mat m_rot_mat;	// found rotation matrix
	Wm5::Vector3d m_trans_vec;	// found translation

protected:	// Local Methods
	bool findFrame();	// find 3d frame


public:
	static bool Read(std::string &file_name, TrackFixBlk *block)
	{
		// 
		try {
			std::ifstream ifs(file_name);
			if(!ifs.good()) {
				std::cout << "TrackFixBlk Read " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_iarchive ia(ifs);

			ia >> BOOST_SERIALIZATION_NVP(*block);
		} 
		catch(std::exception &e) {
			std::cout << "TrackFixBlk Read " << e.what() << '\n';
			return(false);
		}
		return(true);
	}

	static bool Write(TrackFixBlk *block, std::string &file_name)
	{
		try {
			std::ofstream ofs(file_name);
			if(!ofs.good()) {
				std::cout << "TrackFixBlk Write " << "Bad File Open " << file_name << '\n';
				return(false);
			}

			boost::archive::xml_oarchive oa(ofs);

			oa << BOOST_SERIALIZATION_NVP(block);
		} 
		catch(std::exception &e) {
			std::cout << "TrackFixBlk Write " << e.what() << '\n';
			return(false);
		}
		return(true);
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IBlock);
		ar & BOOST_SERIALIZATION_NVP(m_val1);
		ar & BOOST_SERIALIZATION_NVP(m_val2);
	}
};

BOOST_CLASS_VERSION(TrackFixBlk, 0)

#endif

