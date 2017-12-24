
cbuffer cbGlobal : register(b0)
{
    row_major matrix World;
    row_major matrix Projection;
};


struct VS_INPUT
{
    float3 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(float4(input.Pos, 1.f), World);
    output.Pos = mul(output.Pos, Projection);
    output.Uv = input.Uv;
	
    return output;
}