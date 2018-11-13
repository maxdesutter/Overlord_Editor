float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];

Texture2D gDiffuseMap;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};
struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 specular : SV_TARGET2;
	float4 light_accumulation : SV_TARGET3;
};
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};




//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;

	float4 originalPosition = float4(input.pos, 1);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;

	for (int i = 0; i < 4; ++i)
	{
		int boneIndex = (int) input.BoneIndices[i];

		if (boneIndex > -1)
		{
			float boneWeight = input.BoneWeights[i];
			float4x4 boneMatrix = gBones[boneIndex];
				transformedPosition += boneWeight * mul(originalPosition, boneMatrix);
			transformedNormal += boneWeight * mul(input.normal, (float3x3)boneMatrix);
		}
	}

	transformedPosition.w = 1.0f;

	output.pos = mul(transformedPosition, gWorldViewProj);
	output.normal = mul(transformedNormal, (float3x3)gWorld);

	//output.pos = mul(float4(input.pos,1), gWorldViewProj);
	//output.normal = mul(input.normal, (float3x3)gWorld);
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(VS_OUTPUT input) : SV_TARGET{

	PS_OUTPUT output = (PS_OUTPUT)0;

	output.color = gDiffuseMap.Sample(samLinear, input.texCoord);
	output.normal = float4(input.normal, 1.0f);
	output.specular = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.light_accumulation = float4(0.0f, 1.0f, 0.0f, 1.0f);

	return output;
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
