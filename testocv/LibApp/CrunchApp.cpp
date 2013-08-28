// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/08/27)


#include "CrunchApp.h"

#include "PixPtData.h"
#include "ImData.h"
#include "CamCalData.h"

const std::string CrunchApp::PixAppQueueStr = "PixAppQueue";
const std::string CrunchApp::RawAppQueueStr = "RawAppQueue";
const std::string CrunchApp::RawPixPtsStr = "RawPixPts";
const std::string CrunchApp::FinalPixPtsStr = "FinalPixPts";
const std::string CrunchApp::InputImsStr = "InputIms";
const std::string CrunchApp::CamCalsStr = "CamCals";
const std::string CrunchApp::PixGeomsOutStr = "PixGeomsOut";
/*
//----------------------------------------------------------------------------
CrunchApp::CrunchApp ()
{
	m_valid = false;
	m_name = "CrunchApp";

}

CrunchApp::CrunchApp (std::string &name)
{
	m_valid = false;
	m_name = name;

}

//----------------------------------------------------------------------------
// Make the one time objects for running app
bool CrunchApp::onInitialize(void)
{
	// 
	// create data managers for geom processing seqs -- shared
	//GlobalAccess data_access;
	GlobalAccess *m_data_access = setup_pix_seq_data();
	m_pix_data_qu = new AppQueueData(pix_queue_size);
	m_raw_data_qu = new AppQueueData(pix_queue_size);

	// make queue for pix geom objects
	if(!m_data_access->addGlobal(PixAppQueue, m_pix_data_qu)) printf("bad PixAppQueue\n");
	// make queue for raw geom objects
	if(!m_data_access->addGlobal(RawAppQueue, m_raw_data_qu)) printf("bad RawAppQueue\n");

	std::cout << "One Way Queues<Geom3d> ";

	std::cout << "Make Threads " << std::endl;

	std::string pix_file = "pix_seq.xml";
	std::string raw_file = "raw_seq.xml";
	std::string traj_file = "traj_seq.xml";

    m_pix_g3d_producer = new G3d_Producer(pix_file, m_data_access, &m_raw_data_qu->GeomQu());
    m_traj_g3d_consumer = new G3d_Consumer(traj_file, m_data_access, &m_raw_data_qu->GeomQu());

    // Start the threads.
    m_traj_g3d_consume = new boost::thread (*m_traj_g3d_consumer);

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    m_pix_g3d_produce = new boost::thread (*m_pix_g3d_producer);
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
		m_traj_g3d_consume->detach();
		m_pix_g3d_produce->detach();
		// deallocate any objects
		delete m_traj_g3d_consume;
		delete m_pix_g3d_produce;
		delete m_pix_g3d_producer;
		delete m_traj_g3d_consumer;
		delete m_pix_data_qu;
		delete m_raw_data_qu;
		delete m_data_access;
	}

GlobalAccess *CrunchApp::setup_pix_seq_data()
{
	GlobalAccess *data_access = new GlobalAccess;
	// raw pix pts storage
	PixPtData *raw_pts = new PixPtData;
	raw_pts->m_parm1 = 13;
	raw_pts->m_parm2 = 11;
	if(!data_access->addGlobal(RawPixPts, raw_pts)) printf("bad data1\n");
	// final pix pts 
	PixPtData *final_pts = new PixPtData;
	final_pts->m_parm1 = 23;
	final_pts->m_parm2 = 22;
	if(!data_access->addGlobal(FinalPixPts, final_pts)) printf("bad data1\n");
	// dummy ims
	ImData *input_ims = new ImData;
	input_ims->m_parm1 = 33;
	input_ims->m_parm2 = 33;
	if(!data_access->addGlobal(InputIms, input_ims)) printf("bad data2\n");
	// dummy cal
	CamCalData *cam_cal = new CamCalData;
	cam_cal->m_parm1 = 33;
	cam_cal->m_parm2 = 33;
	if(!data_access->addGlobal(CamCals, cam_cal)) printf("bad data2\n");
	// output geom3ds
	Geom3dData *geoms = new Geom3dData;
	if(!data_access->addGlobal(PixGeomsOut, geoms)) printf("bad data2\n");

	return(data_access);
}

*/

