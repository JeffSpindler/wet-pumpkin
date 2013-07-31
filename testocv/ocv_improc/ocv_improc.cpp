/**
**  tjl 2013
*/
#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

/** Global Variables */
const string main_win_name = "TestOCV";

const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

const string min_dist_sl = "MinDist";
const int min_dist_max = 100;
int min_dist_parm = 10;
const string canny_sl = "Canny";
const int canny_max = 200;
int canny_parm = 100;
const string size_sl = "Size";
const int size_max = 100;
int size_parm = 10;
const string min_rad_sl = "MinRad";
const int min_rad_max = 50;
int min_rad_parm = 5;
const string max_rad_sl = "MaxRad";
const int max_rad_max = 100;
int max_rad_parm = 50;

const string thresh_min_sl = "ThreshMin";
const int thresh_min_max = 255;
int thresh_min_parm = 40;
const string thresh_max_sl = "ThreshMax";
const int thresh_max_max = 255;
int thresh_max_parm = 200;
const string thresh_step_sl = "TheshStep";
const int thresh_step_max = 100;
int thresh_step_parm = 20;
const string area_min_sl = "AreaMin";
const int area_min_max = 200;
int area_min_parm = 50;
const string area_max_sl = "AreaMax";
const int area_max_max = 1000;
int area_max_parm = 200;

/** Matrices to store images */
Mat cal_im1;
Mat cal_im2;
Mat gray;


/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
static void on_min_dist_slider( int, void* )
{
	cvtColor(cal_im1, gray, CV_BGR2GRAY);
}

static void on_canny_slider( int, void* )
{
}

bool findBlob(int mode, Mat &gray, vector<Vec3f> &circ_v) 
{
	SimpleBlobDetector::Params parms;
	parms.minThreshold = (float) thresh_min_parm;
	parms.maxThreshold = (float) thresh_max_parm;
	parms.thresholdStep = (float) thresh_step_parm;
	parms.blobColor = 0;
	parms.filterByArea = true;
	parms.minArea = (float) area_min_parm;
	parms.maxArea = (float) area_max_parm;
	parms.filterByConvexity = true;
	parms.minConvexity = 0.5;
	parms.maxConvexity = 1.5;
	parms.minDistBetweenBlobs = (float) min_dist_parm;

	SimpleBlobDetector detector(parms);


	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector.detect( gray, keypoints_1 );

	//-- Draw keypoints
	Mat img_keypoints_1; Mat img_keypoints_2;

	drawKeypoints( gray, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

	//-- Show detected (drawn) keypoints
	imshow(main_win_name, img_keypoints_1 );

	// copy keypts over to pixPt
	//for(int i=0;i<img_keypoints_1.size();i++) {
	//	Vec3f vec;
	//	vec[0] = img_keypoints_1[
	return(true);
}

void drawPts(Mat &view, vector<Vec3f> &circ_v)
{
	Scalar blue(250, 10, 10);
	Scalar red(10, 10, 250);
	Scalar green(10, 250, 10);
	int rad = 6;
	Point cen;
	Point st_pt;
	Point end_pt;

	for(int i=0;i< (int) circ_v.size();i++) {
		printf("(%4.1f,%4.1f) %4.2f", circ_v[i][0], circ_v[i][1], circ_v[i][2]);
		if(i%6 == 5)
			printf("\n");
		rad = cvRound(circ_v[i][2]);
		cen.x = cvRound(circ_v[i][0]);
		cen.y = cvRound(circ_v[i][1]);
		circle(view, cen, rad, red);

		st_pt.x = cen.x + rad;
		st_pt.y = cen.y;
		end_pt.x = cen.x - rad;
		end_pt.y = cen.y;

		line(view, st_pt, end_pt, red);

		st_pt.x = cen.x;
		st_pt.y = cen.y + rad;
		end_pt.x = cen.x;
		end_pt.y = cen.y - rad;

		line(view, st_pt, end_pt, red);
	}

	imshow( main_win_name, view );
}

void drawPts(Mat &view, vector<Point2f> &pointBuf)
{
	Scalar blue(250, 10, 10);
	Scalar red(10, 10, 250);
	Scalar green(10, 250, 10);
	int rad = 6;
	Point st_pt;
	Point end_pt;

	for(int i=0;i<(int) pointBuf.size();i++) {
		printf("(%5.2f,%5.2f) ", pointBuf[i].x, pointBuf[i].y);
		if(i%6 == 5)
			printf("\n");
		circle(view, pointBuf[i], rad, red);
		st_pt = pointBuf[i];
		end_pt = pointBuf[i];
		st_pt.y += rad + 1;
		end_pt.y -= rad + 1;
		line(view, st_pt, end_pt, red);
		st_pt = pointBuf[i];
		end_pt = pointBuf[i];
		st_pt.x += rad + 1;
		end_pt.x -= rad + 1;
		line(view, st_pt, end_pt, red);
	}

	imshow( main_win_name, view );
}

/**
 * @function main
 * @brief Main function
 */
int main( void )
{

	/// Read image ( same size, same type )
	cal_im1 = imread("BD_Cal_Images/Im00100.bmp");
	cal_im2 = imread("BD_Cal_Images/Im00000.bmp");

    cvtColor(cal_im1, gray, CV_BGR2GRAY);

	if( !cal_im1.data ) { printf("Error loading src1 \n"); return -1; }
	if( !cal_im2.data ) { printf("Error loading src2 \n"); return -1; }

	/// Initialize values
	min_dist_parm = 10;
	canny_parm = 100;
	size_parm = 10;
	min_rad_parm = 5;
	max_rad_parm = 50;

	/// Create Windows
	namedWindow(main_win_name, 1);

	/// Create Trackbars
	createTrackbar( min_dist_sl, main_win_name, &min_dist_parm, min_dist_max, on_min_dist_slider );
	createTrackbar( thresh_min_sl, main_win_name, &thresh_min_parm, thresh_min_max );
	createTrackbar( thresh_max_sl, main_win_name, &thresh_max_parm, thresh_max_max );
	createTrackbar( thresh_step_sl, main_win_name, &thresh_step_parm, thresh_step_max );
	createTrackbar( area_min_sl, main_win_name, &area_min_parm, area_min_max );
	createTrackbar( area_max_sl, main_win_name, &area_max_parm, area_max_max );

	//char ButtonName[50] = "HeyB";
	//createButton(ButtonName,callbackButton);//create a push button "button 0", that will call callbackButton.
	/// Show some stuff
	on_min_dist_slider( min_dist_parm, 0 );
	//on_canny_slider( canny_parm, 0 );
	imshow( main_win_name, cal_im1 );

	/// Wait until user press some key

	while(1) {
		int key_hit = waitKey(10);
		if(key_hit == 0x1b)
			break;
		key_hit = toupper(key_hit);
		switch(key_hit) {
			case 'Q': {
				vector<Vec3f> circ_v;
				cvtColor(cal_im1, gray, CV_BGR2GRAY);
				findBlob(0, gray, circ_v);
				}
				break;
			case 'A': {
				vector<Point2f> pointBuf;
				vector<Vec3f> circles;
				//bool found;
				cv::Size board_dim(9,9);

				//found = findCirclesGrid( cal_im1, board_dim, pointBuf );
				cvtColor(cal_im1, gray, CV_BGR2GRAY);

				HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
							 1, min_dist_parm, canny_parm, size_parm, min_rad_parm, max_rad_parm );


				drawPts(gray, circles);
				}
				break;
		}
	}
	return 0;
}
