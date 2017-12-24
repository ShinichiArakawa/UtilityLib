
cbuffer cbGlobal : register(b0)
{
    row_major matrix World;
    row_major matrix View;
    row_major matrix Projection;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    float4 pos = float4(input.Pos.xyz, 1.f);

    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.Pos = pos;

	return output;
}