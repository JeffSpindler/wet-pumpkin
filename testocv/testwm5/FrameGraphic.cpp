// Perform3-D LLC
// Copyright (c) 2013
//
// File Version: 1.0.0 (2013/09/29)

#include "FrameGraphic.h"
#include "Wm5Environment.h"
#include "Wm5StandardMesh.h"
#include "Wm5Texture2DEffect.h"
#include "Wm5Node.h"
#include "Wm5VertexColor3Effect.h"

//----------------------------------------------------------------------------
FrameGraphic::FrameGraphic (Wm5::Node *parent_node, float radius) : mRadius(radius), m_polyseg(NULL)
{
	Float3 org(0,0,0);
	Float3 x_axis(radius,0,0);
	Float3 y_axis(0,radius,0);
	Float3 z_axis(0,0,radius);

    VertexFormat* vformat = VertexFormat::Create(2,
        VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
        VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT3, 0);
    int vstride = vformat->GetStride();

    VertexBuffer* vbuffer = new0 VertexBuffer(6, vstride);
    VertexBufferAccessor vba(vformat, vbuffer);

	vba.Position<Float3>(0) = org;
	vba.Color<Float3>(0, 0) = Float3(1.0, 0, 0);
	vba.Position<Float3>(1) = x_axis;
	vba.Color<Float3>(0, 1) = Float3(1.0, 0, 0);

	vba.Position<Float3>(2) = org;
	vba.Color<Float3>(0, 2) = Float3(0, 1.0, 0);
	vba.Position<Float3>(3) = y_axis;
	vba.Color<Float3>(0, 3) = Float3(0, 1.0, 0);
	vba.Position<Float3>(4) = org;
	vba.Color<Float3>(0, 4) = Float3(0, 0, 1.0);
	vba.Position<Float3>(5) = z_axis;
	vba.Color<Float3>(0, 5) = Float3(0, 0, 1.0);

    m_polyseg = new0 Polysegment(vformat, vbuffer, false);
    m_polyseg->SetEffectInstance(VertexColor3Effect::CreateUniqueInstance());

	if(parent_node != NULL) {
		parent_node->AttachChild(m_polyseg);
	}
}
//----------------------------------------------------------------------------
float FrameGraphic::GetRadius () const
{ 
    return mRadius; 
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
