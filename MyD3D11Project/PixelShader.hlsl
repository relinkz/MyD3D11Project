struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	return input.color;
	// return float4(1.0f, 0.0f, 0.0f, 1.0f);    // Yellow, with Alpha = 1
}