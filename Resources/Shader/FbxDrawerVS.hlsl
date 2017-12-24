cbuffer cbGlobal : register(b0)
{
    row_major matrix World;
    row_major matrix View;
    row_major matrix Projection;
};

cbuffer cbBone : register(b1)
{
    row_major matrix CurrentMatrix[256];
    row_major matrix NextMatrix[256];
    float weight;
    bool isAnimation;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
    float4 BoneWeight : BLENDWEIGHT;
    uint4  BoneIndex : BLENDINDICES;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

matrix BlendCrossFade(matrix current, matrix next, float nextWeight)
{
    float weight = 1.0f - nextWeight;
    return (current * weight) + (next * nextWeight);
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    float4 pos = float4(input.Pos.xyz, 1.f);

    matrix current = CurrentMatrix[input.BoneIndex[0]] * input.BoneWeight[0];
    current += CurrentMatrix[input.BoneIndex[1]] * input.BoneWeight[1];
    current += CurrentMatrix[input.BoneIndex[2]] * input.BoneWeight[2];
    current += CurrentMatrix[input.BoneIndex[3]] * input.BoneWeight[3];

    matrix next = NextMatrix[input.BoneIndex[0]] * input.BoneWeight[0];
    next += NextMatrix[input.BoneIndex[1]] * input.BoneWeight[1];
    next += NextMatrix[input.BoneIndex[2]] * input.BoneWeight[2];
    next += NextMatrix[input.BoneIndex[3]] * input.BoneWeight[3];

    matrix skin = (isAnimation) ? BlendCrossFade(current, next, weight)
    : matrix(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);
    
    pos = mul(pos, skin);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.Pos = pos;
    output.Normal = input.Normal;
    output.UV = input.UV;

	return output;
}