Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbMaterial : register(b0)
{
    float3 emissive; //  !<  自己発光色
    float3 ambient; //  !<  環境光
    float3 diffuse; //  !<  拡散反射光
    float3 specular; //  !<  鏡面反射光
    float shininess; //  !<  強度
    float transparencyFactor; //  !<  透過度
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Noemal : NORMAL;
	float2 UV : TEXCOORD;
};

float4 main( PS_INPUT input) : SV_Target
{
    //  ライトを実装するまで取り敢えずハーフランバート
    float3 normal = normalize(input.Noemal);
    float3 color = emissive + ambient + diffuse + specular;
    color = saturate(color);
    float col = dot(normal, color);

    col = col * 0.5f + 0.5f;
    col = col * col;

    return tex.Sample(samLinear, input.UV) * col;
}