struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 norm : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float4 vLightDir = float4(0.0f, 0.0f, -1.0f, 1.0f);
	float4 vLightColor = float4(0.5f, 0.0f, 0.0f, 1.0f);

	//float4 vAmbient = float4();

	//return input.norm;
	//float4 toReturn = 0.0f;
	//toReturn +=saturate( dot( (float3)vLightDir, input.norm) * vLightColor);

	return float4(abs(input.norm), 0.0f);
	
	//return toReturn;
	//return saturate(dot((float3)vLightDir, input.norm) * vLightColor;
}