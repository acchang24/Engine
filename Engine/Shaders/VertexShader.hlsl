struct VSOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VSOut main(float2 pos : POSITION, float4 color : COLOR) 
{
	VSOut output;

	output.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	output.color = color;

	return output;
}

//float4 main( float2 pos : POSITION, float3 color : COLOR ) : SV_POSITION
//{
//	return float4(pos.x, pos.y, 0.0f, 1.0f);
//}