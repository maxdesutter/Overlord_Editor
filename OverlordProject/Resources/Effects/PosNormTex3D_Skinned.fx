float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];
bool gHasBones = false;
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

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

    //VS_OUTPUT output1;
	//// Step 1:	convert position into float4 and multiply with matWorldViewProj
    //output1.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
	//// Step 2:	rotate the normal: NO TRANSLATION
	////			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	////			thus removing the postion row of the matrix
    //output1.normal = normalize(mul(input.normal, (float3x3) gWorld));
    //output1.texCoord = input.texCoord;
    //return output1;




    
	VS_OUTPUT output;
    output.pos = float4(0,0,0,0);
    output.normal = float3(0, 0, 0);

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
    
	output.pos += mul(transformedPosition, gWorldViewProj) * gHasBones;
    output.normal += mul(transformedNormal, (float3x3) gWorld) * gHasBones;

    output.pos += mul(float4(input.pos, 1), gWorldViewProj) * (1-gHasBones);
    output.normal += mul(input.normal, (float3x3) gWorld) * (1 - gHasBones);
	output.texCoord = input.texCoord;
	return output;

}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb , color_a );
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

