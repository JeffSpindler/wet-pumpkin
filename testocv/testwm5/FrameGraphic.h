// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/29)

#ifndef FRAMEGRAPHIC_H
#define FRAMEGRAPHIC_H

#include "Wm5RigidBody.h"
#include "Wm5PolySegment.h"
#include "Wm5Node.h"

using namespace Wm5;


class FrameGraphic : public RigidBodyf
{
public:
    FrameGraphic (Wm5::Node *parent_node, float radius = 5.0f);

    float GetRadius () const;

private:
	Wm5::PolysegmentPtr m_polyseg;
    float mRadius;
};

#endif
