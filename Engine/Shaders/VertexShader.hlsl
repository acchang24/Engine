#include "ShaderConstants.hlsli"

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VertexOut main(VertexIn vIn)
{
	VertexOut output;

	output.pos = mul(float4(vIn.pos.x, vIn.pos.y, vIn.pos.z, 1.0f), modelToWorld);
	output.color = vIn.color;

	return output;
}