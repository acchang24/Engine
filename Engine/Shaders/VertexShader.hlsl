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