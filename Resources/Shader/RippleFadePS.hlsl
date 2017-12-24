Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbFaed : register(b0)
{
    float2 Center;
    float Speed;
    float MoveUvPower;
  
    float WaveNumPower;
    float Alpha;
    float AlphaPower;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    float2 Offset = input.Uv - Center;

    float2 NormalVectol = normalize(Offset);
    float Len = length(Offset);

    float MoveTexcoord = sin(Len * WaveNumPower + Speed) * MoveUvPower;

    float2 ResultTexcoord;
    ResultTexcoord.x = input.Uv.x + MoveTexcoord * NormalVectol.x;
    ResultTexcoord.y = input.Uv.y + MoveTexcoord * NormalVectol.y;
	
    float4 Color = tex.Sample(samLinear, ResultTexcoord);
    Color.a = (Alpha - Len * AlphaPower);

    return Color;
}