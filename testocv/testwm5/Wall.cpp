// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/09/29)

#include "Wall.h"

#include "Wm5VertexColor3Effect.h"

//
// make a wall
//

Wall::Wall (Wm5::Node *parent_node, Vector3f &pos, Vector3f &size, Float3 &color)
{
    VertexFormat* vformat = VertexFormat::Create(2,
        VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
        VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0);
    int vstride = vformat->GetStride();

    Float3 sideWall2Color(170.0f/255.0f, 187.0f/255.0f, 219.0f/255.0f);

    VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
    VertexBufferAccessor vba(vformat, vbuffer);
	if(size[0] == 0) {
		vba.Position<Float3>(0) = Float3(pos[0], pos[1]-size[1],  pos[2]-size[2]);
		vba.Position<Float3>(1) = Float3(pos[0], pos[1]+size[1],  pos[2]-size[2]);
		vba.Position<Float3>(2) = Float3(pos[0], pos[1]+size[1],  pos[2]+size[2]);
		vba.Position<Float3>(3) = Float3(pos[0], pos[1]-size[1],  pos[2]+size[2]);
	} else if(size[1] == 0) {
		if(pos[1] > 0) {
			vba.Position<Float3>(0) = Float3(pos[0]-size[0], pos[1],  pos[2]-size[2]);
			vba.Position<Float3>(1) = Float3(pos[0]+size[0], pos[1],  pos[2]-size[2]);
			vba.Position<Float3>(2) = Float3(pos[0]+size[0], pos[1],  pos[2]+size[2]);
			vba.Position<Float3>(3) = Float3(pos[0]-size[0], pos[1],  pos[2]+size[2]);
		} else {
			vba.Position<Float3>(0) = Float3(pos[0]+size[0], pos[1],  pos[2]-size[2]);
			vba.Position<Float3>(1) = Float3(pos[0]-size[0], pos[1],  pos[2]-size[2]);
			vba.Position<Float3>(2) = Float3(pos[0]-size[0], pos[1],  pos[2]+size[2]);
			vba.Position<Float3>(3) = Float3(pos[0]+size[0], pos[1],  pos[2]+size[2]);
		}
	} else {
		vba.Position<Float3>(0) = Float3(pos[0]-size[0], pos[1]-size[1],  pos[2]);
		vba.Position<Float3>(1) = Float3(pos[0]+size[0], pos[1]-size[1],  pos[2]);
		vba.Position<Float3>(2) = Float3(pos[0]+size[0], pos[1]+size[1],  pos[2]);
		vba.Position<Float3>(3) = Float3(pos[0]-size[0], pos[1]+size[1],  pos[2]);
	}
    vba.Color<Float3>(0,0) = color;
    vba.Color<Float3>(0,1) = color;
    vba.Color<Float3>(0,2) = color;
    vba.Color<Float3>(0,3) = color;

    IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
    int* indices = (int*)ibuffer->GetData();
    indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
    indices[3] = 0;  indices[4] = 2;  indices[5] = 3;

    m_mesh = new0 TriMesh(vformat, vbuffer, ibuffer);
    m_mesh->SetEffectInstance(VertexColor3Effect::CreateUniqueInstance());

	if(parent_node != NULL) {
		parent_node->AttachChild(m_mesh);
	}
}



