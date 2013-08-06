// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/08/04)

//----------------------------------------------------------------------------
Geom3d::Geom3d ()  : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0)
{
	m_valid = true;
	m_type = POINT;
	memset(m_pt, 0, sizeof(m_pt));
	memset(m_vel, 0, sizeof(m_vel));
	memset(m_unit, 0, sizeof(m_unit));
}
//----------------------------------------------------------------------------
Geom3d::~Geom3d ()
{
}
//----------------------------------------------------------------------------
Geom3d::Geom3d (double pos[]) : m_tag(0), m_idx(0), m_time_usec(0), m_conf(0), m_valid(true)
{
	m_type = POINT;
	m_pt[0] = pos[0]; m_pt[1] = pos[1]; m_pt[2] = pos[2];
	memset(m_vel, 0, sizeof(m_vel));
	memset(m_unit, 0, sizeof(m_unit));
}
//----------------------------------------------------------------------------
