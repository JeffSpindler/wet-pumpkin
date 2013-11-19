// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/08/27)

#include "Wm5Vector3.h"
#include "Wm5Ray3.h"
using namespace Wm5;

#include "CrunchApp.h"

//----------------------------------------------------------------------------
// Make the one time objects for running app
//
// Includes ExtCmd object to provide external access to operations and data
//
bool CrunchApp::onInitialize(void)
{ 
	std::cout << "onInitialize " << m_name << '\n';
	// create data managers for geom processing seqs -- shared
	//GlobalAccess data_access;
	GlobalAccess *m_data_access = setup_global_data();

	m_pix_data_qu = new AppQueueData(pix_queue_size);
	m_traj_data_qu = new AppQueueData(pix_queue_size);

	// make queue for pix geom objects
	if(!m_data_access->addGlobal(GlobalAccess::PixAppQuStr, m_pix_data_qu)) printf("bad PixAppQueue\n");
	// make queue for raw geom objects
	if(!m_data_access->addGlobal(GlobalAccess::TrajAppQuStr, m_traj_data_qu)) printf("bad TrajAppQueue\n");

	std::cout << "One Way Queues<Geom3d> ";

	std::cout << "Make Threads " << std::endl;

	//std::string pix_file = "pix_seq.xml";
	std::string traj_file = "traj_seq.xml";

	m_traj_g3d_processor = new Geom3d_Processor(traj_file, m_data_access, &(m_pix_data_qu->GeomQu()), &(m_traj_data_qu->GeomQu()) );

	// Start the threads.
	m_traj_g3d_process = new boost::thread (*m_traj_g3d_processor);

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	//while(1);
	std::cout << "Threads Active " << std::endl;	
	m_valid = true;
	return(m_valid); 
}
// call this to kill everything
void CrunchApp::OnTerminate()
{
	if(!m_valid)
		return;

	// stop the threads
	m_traj_g3d_process->detach();
	// deallocate any objects
	delete m_traj_g3d_process;
	delete m_traj_g3d_processor;
	delete m_pix_data_qu;
	delete m_traj_data_qu;

	//delete m_data_access;
	std::cout << "OnTerminate " << m_name << '\n';
}

GlobalAccess *CrunchApp::setup_global_data()
 { 
	GlobalAccess *data_access = new GlobalAccess;
	
	// geom queues

	// geom processing storage
	Geom3dData *in_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::InGeomStr, in_geoms)) printf("bad data2\n");

	Geom3dData *camray_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::CamRayStr, camray_geoms)) printf("bad data2\n");

	Geom3dData *cur_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::CurGeomStr, cur_geoms)) printf("bad data2\n");

	Geom3dData *out_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::OutGeomStr, out_geoms)) printf("bad data2\n");

	// other geom storage
	Geom3dData *fix_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::FixStr, fix_geoms)) printf("bad data2\n");
	Geom3dData *model_geoms = new Geom3dData;
	if(!data_access->addGlobal(GlobalAccess::ModelStr, model_geoms)) printf("bad data2\n");

	testModel(11, model_geoms->Geoms());

	// raw pix pts storage
	PixPtData *raw_pts = new PixPtData;
	raw_pts->m_parm1 = 13;
	raw_pts->m_parm2 = 11;
	if(!data_access->addGlobal(GlobalAccess::RawPixPtsStr, raw_pts)) printf("bad data1\n");
	// final pix pts 
	PixPtData *final_pts = new PixPtData;
	final_pts->m_parm1 = 23;
	final_pts->m_parm2 = 22;
	if(!data_access->addGlobal(GlobalAccess::FinalPixPtsStr, final_pts)) printf("bad data1\n");
	// dummy ims
	ImData *input_ims = new ImData;
	input_ims->m_parm1 = 33;
	input_ims->m_parm2 = 33;
	if(!data_access->addGlobal(GlobalAccess::InputImsStr, input_ims)) printf("bad data2\n");
	// dummy cal
	CamCalData *cam_cal = new CamCalData;
	cam_cal->m_parm1 = 33;
	cam_cal->m_parm2 = 33;
	if(!data_access->addGlobal(GlobalAccess::CamCalsStr, cam_cal)) printf("bad data2\n");
	// geom3d queues




	return(data_access);
}

bool CrunchApp::testPixSeq(int mode)
{
	std::string SeqFileSavStr = "pix_seq_sav.xml";
	
	std::cout << "testPixSeq Read/Write Blocks\n\n" << std::endl;

	AcqImBlk *blockacq = new AcqImBlk;
	//blockacq->Name() = "";
	blockacq->print();

	// thresh and rle
	// cca
	// select best

	// make into camrays
	PixCandsBlk *block_cands = new PixCandsBlk;
	//blockacq->Name() = "";
	block_cands->print();

	// make the seq and add the blocks
	AppSeq pix_seq;

	pix_seq.insertBlock(blockacq);
	pix_seq.insertBlock(block_cands);

	pix_seq.print();


	std::cout << "\n Write Seq\n";
	// write out the resulting seq

	AppSeq::Write(&pix_seq, SeqFileSavStr);

	// read back and print
	//AppSeq *rd_seq = new AppSeq;
	//if(AppSeq::Read(SeqFileSavStr, rd_seq)) {
	//	rd_seq->print();
	//}
	std::cout << std::endl << std::endl;
	return(false);
}

bool CrunchApp::testTrajSeq(int mode)
{
	std::string SeqFileSavStr = "traj_seq_sav.xml";

	// get an image
	std::cout << "testTrajSeq Read/Write Blocks\n\n" << std::endl;

	// load fixtures to locate
	LoadFixBlk *load_fix = new LoadFixBlk;
	load_fix->print();

	// sort input rays by fixtures -- setup cands for labeling
	SortFixBlk *sort_fix = new SortFixBlk;
	sort_fix->print();

	// label cands for all fixtures
	LabelFixBlk *label_fix = new LabelFixBlk;
	label_fix->print();

	// Track all fixtures -- create frame for each fix
	TrackFixBlk *track_fix = new TrackFixBlk;
	label_fix->print();

	// make the seq and add the blocks
	AppSeq traj_seq;

	traj_seq.insertBlock(load_fix);
	traj_seq.insertBlock(sort_fix);
	traj_seq.insertBlock(label_fix);
	traj_seq.insertBlock(track_fix);

	// save and print result
	std::cout << "\n Write Traj Seq\n";
	AppSeq::Write(&traj_seq, SeqFileSavStr);
	traj_seq.print();
	std::cout << std::endl << std::endl;
	return(true);
}

// stuff a dq with a set of camrays
// Just find dir from cam to pts
bool CrunchApp::testCamRay(int frame_num, Geom3d_v_t &model_v, Geom3d_dq_t &g3d_dq)
{
	//const int num_pts = 3;
	//double pos[num_pts][3] = { {10, 0, 0}, {0,10,0}, {0,0,10}};
	const int num_cams = 2;
	double cam_cen[num_cams][3] = { {100, 200, 250}, {100,-200,250} };
	Vector3d Dir;
	std::cout << std::endl << "testCamRay" << std::endl;
	g3d_dq.clear();
	for(unsigned c=0;c<num_cams;c++) {
		Vector3d cam_cen_vec(cam_cen[c][0], cam_cen[c][1], cam_cen[c][2]);
		for(unsigned i=0;i<model_v.size();i++) {
			Vector3d pt(model_v[i].m_pt[0], model_v[i].m_pt[1], model_v[i].m_pt[2]);
			Dir = pt - cam_cen_vec;
			Dir.Normalize();
			Geom3d Ray(cam_cen[c]);
			Ray.m_type = Geom3d::CAM_RAY;
			Ray.m_tag = model_v[i].m_tag;
			Ray.m_idx = i;
			Ray.m_src = c;
			Ray.m_conf = 1.0;
			Ray.m_dir[0] = Dir[0];
			Ray.m_dir[1] = Dir[1];
			Ray.m_dir[2] = Dir[2];
			g3d_dq.push_back(Ray);
			if(c == 0) std::cout << pt << std::endl;
		}
	}
	return(!g3d_dq.empty());
}

// stuff a dq with a set of pts
bool CrunchApp::testModel(int frame_num, Geom3d_v_t &model_v)
{
	const int num_pts = 3;
	double pos[num_pts][3] = { {50, 0, 0}, {50,30,0}, {50,0,70}};
	std::cout << std::endl << "testModel" << std::endl;

	model_v.clear();
	for(int i=0;i<num_pts;i++) {
		Geom3d pt(pos[i][0], pos[i][1], pos[i][2]);
		pt.m_type = Geom3d::POINT;
		pt.m_tag = i;
		pt.m_idx = i;
		pt.m_src = 3;
		pt.m_conf = 1.0;
		model_v.push_back(pt);
		std::cout << pt << std::endl;
	}
	return(!model_v.empty());
}




