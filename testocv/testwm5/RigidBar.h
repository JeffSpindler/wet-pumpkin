// Geometric Tools, LLC
// Copyright (c) 1998-2013
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#ifndef RIGIDBAR_H
#define RIGIDBAR_H

#include "Wm5Environment.h"
#include "Wm5StandardMesh.h"
#include "Wm5Texture2DEffect.h"
#include "Wm5RigidBody.h"
#include "Wm5TriMesh.h"
#include "Wm5Node.h"


class RigidBar : public RigidBodyf
{
public:
    RigidBar (Wm5::Node *parent_node, float radius = 1.0f) : Moved(false), mRadius(radius)
    {
        float len_ratio = 44.0;

        Wm5::VertexFormat* vformat = Wm5::VertexFormat::Create(2,
            Wm5::VertexFormat::AU_POSITION, Wm5::VertexFormat::AT_FLOAT3, 0,
            Wm5::VertexFormat::AU_TEXCOORD, Wm5::VertexFormat::AT_FLOAT2, 0);

        mMesh = StandardMesh(vformat).Cylinder(16, 16, mRadius, mRadius*len_ratio, false);

        std::string path = Wm5::Environment::GetPathR("Rope.wmtf");
        Wm5::Texture2D* texture = Wm5::Texture2D::LoadWMTF(path);
        mMesh->SetEffectInstance(Wm5::Texture2DEffect::CreateUniqueInstance(texture,
            Wm5::Shader::SF_LINEAR, Wm5::Shader::SC_CLAMP_EDGE, Wm5::Shader::SC_CLAMP_EDGE));

	    if(parent_node != NULL) {
		    parent_node->AttachChild(mMesh);
	    }
    };

    Wm5::TriMeshPtr& Mesh ()
    {
        return mMesh;
    };

    float GetRadius () const
    { 
        return mRadius; 
    };

    bool Moved;

private:
    Wm5::TriMeshPtr mMesh;
    float mRadius;

};

#endif
