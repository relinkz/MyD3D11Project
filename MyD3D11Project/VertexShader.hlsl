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
};

VS_OUTPUT main(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(Pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	//output.pos = Pos;
	output.color = Color;

	return output;
}