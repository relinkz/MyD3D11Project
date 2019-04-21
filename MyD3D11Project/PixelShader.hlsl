struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 norm : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 vLightDir = float4(0.0f, 0.0f, -1.0f, 1.0f);
	float4 vLightColor = float4(0.5f, 5.0f, 5.0f, 1.0f);

	return float4(abs(input.norm),1.0f);
	return input.color;
}