// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/02)

#include "TrackFixBlk.h"
#include "GlobalAccess.h"

#include "Pt3d.h"
#include "Mat.h"

#include "Wm5Vector3.h"
#include "Wm5SingularValueDecomposition.h"

using namespace Wm5;

	
// Given fixtures found, track them and find the current frames for each

bool TrackFixBlk::doSetup(IGlobalAccess *data_access) 
{ 
	if(!data_access)	return false;
	m_fix = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::FixStr));
	m_model = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::ModelStr));
	m_camrays = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CamRayStr));
	m_cur = reinterpret_cast<Geom3dData*>(data_access->getGlobal(GlobalAccess::CurGeomStr));

	m_cals = reinterpret_cast<CamCalData*>(data_access->getGlobal(GlobalAccess::CamCalsStr));
	m_pts = reinterpret_cast<PixPtData*>(data_access->getGlobal(GlobalAccess::FinalPixPtsStr));

	// setup model for frames
	m_num_mod_pts = m_model->Geoms().size();
	m_mod_com.SetSize(3);
	m_mod_pt_mat.setName("Mod Pt Mat");
	m_wt_mat.setName("Wt Mat");
	m_cur_pt_mat.setName("Cur Pt Mat");
	BOOST_FOREACH(Geom3d g3d, m_model->Geoms()) {
		m_mod_pt_map[g3d.m_tag] = GVectord(3, g3d.m_pt);
		m_mod_wt_map[g3d.m_tag] = g3d.m_conf;
	}

	return (m_camrays != NULL && m_fix != NULL && m_model != NULL && m_cur != NULL);
};

bool TrackFixBlk::doRun(IGlobalAccess *data_access) 
{ 
	if(Debug(3)) {
		std::cout << boost::format(" %s model %d cur %d\n") % m_name % m_num_mod_pts % m_cur->Geoms().size(); 
		if(m_cur != NULL) m_cur->print();
	}

	findFrame();

	return(true);
};

void TrackFixBlk::doPrint() 
{ 
	std::cout << m_name << std::endl;

	if(m_cals != NULL) m_cals->print();
	if(m_pts != NULL) m_pts->print();

	if(m_cur != NULL) m_cur->print();
	std::cout << std::endl;
};

// this is taken from:  Least-Squares Rigid Motion Using SVD by Olga Sorkine
//
// have model (m_model) nd labeled pts (m_cur) find the min err frame shift
// use confidence as wts
//
// need cur pts to contain <= mod_pts with the m_idx matching the model m_idx
bool TrackFixBlk::findFrame()
{
	int num_pts = m_model->Geoms().size();
	if(num_pts < 3) return(false);

	m_mod_pt_mat.SetSize(num_pts, 3);
	m_cur_pt_mat.SetSize(num_pts, 3);
	m_wt_mat.SetSize(num_pts, num_pts);
	m_covar_mat.SetSize(num_pts, num_pts);
	m_Diag_mat.SetSize(num_pts, num_pts);
	
	Pt3d mod_com;
	Pt3d cur_com;

	double wts_sum = 0;
	// first setup wt mat with cur and mod wts 
	// -- do all cur pts with 0 wt if no mod
	for(int i=0;i<num_pts;i++) {
		int tag = m_cur->Geoms()[i].m_tag;
		if(m_mod_wt_map.find(tag) == m_mod_wt_map.end()) {
			m_wt_mat[i][i] = 0;
		} else {
			m_wt_mat[i][i] = m_mod_wt_map[tag] * m_cur->Geoms()[i].m_conf;
		}
		// now setup the coms for mod and cur
		m_mod_com += m_mod_pt_map[i] * m_cur->Geoms()[i].m_conf;
		m_Diag_mat[i][i] = 1;
	}

	//if(wts_sum == 0) return
	// weighted centroids
	m_mod_com /= wts_sum;
	m_cur_com /= wts_sum;

	// now setup model part of covariance matrix

	for(int i=0;i<num_pts;i++) {
		int tag = m_cur->Geoms()[i].m_tag;
		// setup cols for cur and mod
		m_cur_pt_mat.SetColumn(i, GVectord(3, m_cur->Geoms()[i].m_pt));	// set to zeros if not valid
		m_mod_pt_mat.SetColumn(i, m_mod_pt_map[tag]);	// set to zeros if not valid
	}

	m_covar_mat = m_cur_pt_mat * m_wt_mat * (m_mod_pt_mat.Transpose());

	if(Debug(3)) {
		std::cout << m_mod_pt_mat;
		std::cout << m_cur_pt_mat;
		std::cout << m_wt_mat;
		std::cout <<  Mat("CoVar Mat", m_covar_mat);
	}

	SingularValueDecompositiond findSVD(m_covar_mat, m_svd_U, m_svd_Diag, m_svd_VT);
	Mat V_mat("V Mat", m_svd_VT.Transpose());
	Mat U_T_mat("U_T Mat", m_svd_U.Transpose());

	if(Debug(3)) {
		std::cout << V_mat;
		std::cout << m_svd_Diag;
		std::cout << U_T_mat;
	}
	// find sign of last diagonal entry
	// this results in a 3x3, so copy to a Matrix3d and find det
	Mat V_U_T_mat("V_UT");
	V_U_T_mat = V_mat * U_T_mat;
	Matrix3d V_U_T_mat3(V_U_T_mat.GetElements(), true);
	m_Diag_mat[num_pts-1][num_pts-1] = V_U_T_mat3.Determinant();

	m_rot_mat = V_mat * m_Diag_mat * U_T_mat;
	if(Debug(3)) {
		std::cout <<  V_U_T_mat;
		std::cout << Mat("Diag Mat", m_Diag_mat);
		std::cout << Mat("Rot Mat", m_rot_mat);
	}

	return(true);
}

//BOOST_CLASS_EXPORT(TrackFixBlk);
