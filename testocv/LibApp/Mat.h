// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/04)

#ifndef MAT_H
#define MAT_H

#include "Global.h"

#include "Wm5GMatrix.h"
#include "Wm5Matrix3.h"
#include "Wm5Matrix4.h"

class Mat : public Wm5::GMatrixd
{
public:
    // This is based on a WM Vector3d and a tag object with addl data required for use.
	//

    // Construction and destruction.
    Mat ()  : m_name("Mat") {};
    Mat (std::string name)  : m_name(name) {};
    Mat (int rows, int cols) :  Wm5::GMatrixd(rows, cols) {};
    Mat (int rows, int cols, const double *entry) :  Wm5::GMatrixd(rows, cols, entry) {};
    Mat (int rows, int cols, const double** matrix) :  Wm5::GMatrixd(rows, cols, matrix) {};
    Mat (std::string name, int rows, int cols) :  m_name(name), Wm5::GMatrixd(rows, cols) {};
    Mat (std::string name, int rows, int cols, const double *entry) : m_name(name), Wm5::GMatrixd(rows, cols, entry) {};
    Mat (std::string name, int rows, int cols, const double** matrix) :  m_name(name), Wm5::GMatrixd(rows, cols, matrix) {};
    Mat (std::string name,  Wm5::GMatrixd& mat) :  Wm5::GMatrixd(mat), m_name(name) {};
    Mat (const Wm5::GMatrixd& mat) :  Wm5::GMatrixd(mat) {};
    Mat (const Mat& mat) :  Wm5::GMatrixd(mat) {};
	~Mat () {};

	std::string m_name;

	void setName(std::string name) { m_name = name; };

	friend std::ostream& operator<<(std::ostream& os, const Mat &mat) {

		os << boost::format("%s %dx%d\n") % mat.m_name % mat.GetNumRows() % mat.GetNumColumns(); 
		for(int i=0; i<mat.GetNumColumns();i++) {
			os << "| ";
			for(int j=0;j<mat.GetNumRows(); j++) {
				os << boost::format(" %8.2f") % mat[i][j];
			}
			os << " |" << std::endl;
		}
		os << std::endl; 
		return os;
	}
private:
	// Serialization Support
	friend class boost::serialization::access;
	template<class Archive>	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(m_name);
	}
};

typedef std::vector<Mat> Mat_v_t;
typedef std::deque<Mat> Mat_dq_t;

BOOST_CLASS_VERSION(Mat, 0)

#endif
