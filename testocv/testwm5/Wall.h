// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/29)

#ifndef WALL_H
#define WALL_H

#include "Wm5Node.h"
#include "Wm5Vector3.h"
#include "Wm5TriMesh.h"

using namespace Wm5;

class Wall : public Node
{
public:
    Wall (Wm5::Node *parent_node, Vector3f &pos, Vector3f &size, Float3 &color);

	Vector3f GetPos () const { return m_pos; };
	Vector3f GetSize () const { return m_size; };
	TriMeshPtr& Mesh () { return m_mesh; };

private:
    Vector3f m_pos;
	Vector3f m_size;
	Float3 m_color;

private:
    TriMeshPtr m_mesh;
};

typedef Pointer0<Wall> WallPtr;

#endif
