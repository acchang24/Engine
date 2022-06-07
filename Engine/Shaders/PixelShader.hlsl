#include "VertexShader.hlsl"

float4 main(VSOut psIn) : SV_TARGET
{
	return psIn.color;
}