Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbFaed : register(b0)
{
    float2 DrawRegion;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = float4(tex.Sample(samLinear, input.Uv).xyz, 0);
    float fadein = 1.f, fadeout = 0.f;

    if (input.Uv.x < DrawRegion.x)
        color.w += fadein;
    else
        color.w += fadeout;

    if (input.Uv.y < DrawRegion.y)
        color.w += fadein;
    else
        color.w += fadeout;

    color.w *= 0.5f;

    return color;
}