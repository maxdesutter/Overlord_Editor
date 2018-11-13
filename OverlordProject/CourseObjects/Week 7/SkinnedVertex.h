#pragma once
#include "VertexHelper.h"

struct SkinnedVertex
{
	SkinnedVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT4 color, float blendWeight0 = .0f, float blendWeight1 = .0f) :
	TransformedVertex(position, normal, color),
	OriginalVertex(position, normal, color), 
	//PART 3
	BlendWeight0(blendWeight0), 
	BlendWeight1(blendWeight1)
	{}

	VertexPosNormCol TransformedVertex;
	VertexPosNormCol OriginalVertex;

	//PART 3
	float BlendWeight0;
	float BlendWeight1;
};