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

float3 main(VS_OUTPUT input) : SV_Target
{
    float4 ambientLight = float4(0.3, 0.0, 0.0, 0.0);
    float4 lightDir = normalize(input.posForLight); // posForLight pos is relative to pixel I assume?
    float angle		= dot(float4(input.norm, 0), lightDir);

    float4 light = ambientLight;
    if(angle > 0.0f)
    {
        light = ambientLight + (angle * input.color);
    }

    return light;
}