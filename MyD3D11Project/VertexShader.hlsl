cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float3 norm : TEXCOORD0;
};

VS_OUTPUT main(float4 Pos : POSITION, float4 Color : COLOR, float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(Pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	// Put the last to 0
	Norm[3] = 0.0f;
	Norm = mul(Norm, world);
	Norm = mul(Norm, view);
	Norm = normalize(Norm);

	output.norm = float3(Norm.x, Norm.y, Norm.z);
	output.color = Color;

	return output;
}