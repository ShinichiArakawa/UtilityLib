struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 color = float4(0.5f, 0.5f, 0.5f, 1);
    return color;
}