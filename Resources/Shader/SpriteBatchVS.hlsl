//  hlslの行列は格納方式を省略すると列優先になる
//  XMFLOAT4xは行優先なのでここで格納方式を指定するか
//  CPUで転置行列を作成する
//  row_major    !<  行ベクトルで格納
//  column_major !<  列ベクトルで格納
cbuffer cbGlobal : register(b0)
{
    row_major matrix World;
    row_major matrix Projection;
};

cbuffer cbTextureInfo : register(b1)
{
    float2 UVPos;
    float2 UVSize;
    float4 Color;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.Pos = mul(float4(input.Pos, 1.f), World);
    output.Pos = mul(output.Pos, Projection);
    output.UV = input.UV * UVSize + UVPos;
    output.Color = Color;

    return output;
}