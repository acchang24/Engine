cbuffer ObjConstants
{
	row_major float4x4 modelToWorld;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VSOut main(float2 pos : POSITION, float4 color : COLOR) 
{
	VSOut output;

	output.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f),modelToWorld);
	output.color = color;

	return output;
}