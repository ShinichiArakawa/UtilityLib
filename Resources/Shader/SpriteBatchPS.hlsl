
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = tex.Sample(samLinear, input.UV);
    color *= input.Color;

    return color;
}