#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"
#include "utils.hlsli"

//이것들은 Shader.cpp 에서 넣어준다.
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;

};

//정점 셰이더
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    //output.color = input.color;
    output.uv = input.uv;

    output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;//빛 연산에 쓰기 위해서 프로젝션 단계를 하기 이전의 값을 보내준다.
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);
    output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);
    output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

    return output;
}

//픽셀 셰이더
float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f,1.f,1.f,1.f);

    //혹시나 노말 없이 사용할 때를 대비해서 만들어준 변수 셰이더에서는 널체크가 불가능 하기 때문에 이렇게 파라미터를 통해 받아와야 한다.
    if(g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    //턱스쳐를 설정
    //float4 
    //float4 color = float4(1.f,1.f,1.f,1.f);

    float3 viewNormal = input.viewNormal;

    if (g_tex_on_1)
    {
        // [0,255] 범위에서 [0,1]로 변환
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        // [0,1] 범위에서 [-1,1]로 변환
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));

    }

    LightColor totalColor = (LightColor)0.f;

    //각각의 픽셀마다 빛에대한 컬러들을 구해서 세팅해준다.
    for (int i = 0; i < g_lightCount; ++i)
    {
        LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
        totalColor.diffuse += color.diffuse;
        totalColor.ambient += color.ambient;
        totalColor.specular += color.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;

    return color;
}

#endif 