cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightPos;
	float4 lightColor;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 norm : TEXCOORD0;
	float4 posForLight : COLOR1;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	float3 vLightDir = float3(lightPos.x, lightPos.y, lightPos.z);
	//float3 vLightDir = float3(input.posForLight.x, input.posForLight.y, input.posForLight.z);

	float3 vCol = float3(input.color.x, input.color.y, input.color.z);
	float3 norm = normalize(input.norm);

	return float4(abs(input.norm), 0);

	vLightDir = vLightDir - float3(input.pos.x, input.pos.y, input.pos.z);

	//vLightDir = input.pos - lightPos;
	//vLightDir = input.posForLight - lightPos;

	vLightDir = normalize(vLightDir);
	//return float4(abs(vLightDir),0);

	float3 ambientLight = float3(0.0, 0.0, 0.0) * vCol;
	float3 diffuseLight = float3(0.0, 0.0, 0.0);
	//float3 specularLight = float3(0.0, 0.0, 0.0);

	float lightDotNorm = acos(dot(vLightDir, norm));
	if (lightDotNorm > 0)
	{
		float3 projLight = saturate(lightDotNorm);
		diffuseLight = projLight * lightColor * vCol;
	}
	//return float4(lightDotNorm*vCol, 0);

	//float3 projLight = saturate(lightDotNorm);
	//float3 diffuseLight = projLight * lightColor * vCol;
	//float specularLight = 0.5 * (lightDotNorm) * (vLightDir - norm) * vCol;
	//return lightColor;
	
	ambientLight = float3(0.0, 0.0, 0.0) * vCol;
	return float4(ambientLight + diffuseLight, 0);
	//return float4(ambientLight + diffuseLight + specularLight,0);
}