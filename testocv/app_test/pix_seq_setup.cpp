#include <cstdio>
#include <iostream>
#include <fstream>

#include "AppSeq.h"

//#include "AppDataOne.h"
//#include "DoABlock.h"
//#include "DoBBlock.h"

#include "AcqImBlk.h"
#include "PixCandsBlk.h"

#include "PixPtData.h"
#include "ImData.h"
#include "CamCalData.h"

#include "GlobalAccess.h"

#include "app_test.h"



std::string RawPixPts = "RawPixPts";
std::string FinalPixPts = "FinalPixPts";
std::string InputIms = "InputIms";
std::string CamCals = "CamCals";
std::string PixGeomsOut = "PixGeomsOut";

GlobalAccess *setup_pix_seq_data()
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

// create a test pix_seq
void create_pix_seq()
{




	// get an image
	
	std::cout << "Read/Write Blocks\n\n" << std::endl;

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

	// save an empty seq
	std::string SeqFileSavStr = "traj_seq_sav.xml";
	AppSeq::Write(&pix_seq, SeqFileSavStr);


	pix_seq.insertBlock(blockacq);
	pix_seq.insertBlock(block_cands);

	//pix_seq.setup(&data_access);
	pix_seq.print();


	std::cout << "\n Write Seq\n";

	SeqFileSavStr = "pix_seq_sav.xml";
	std::string SeqFileStr = "pix_seq.xml";

	// write out the resulting seq

	AppSeq::Write(&pix_seq, SeqFileSavStr);

	AppSeq *rd_seq = new AppSeq;

	if(AppSeq::Read(SeqFileStr, rd_seq)) {
		rd_seq->print();
	}

	std::cout << std::endl << std::endl;

}

