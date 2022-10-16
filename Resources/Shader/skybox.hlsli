#ifndef _SKYBOX_HLSLI_
#define _SKYBOX_HLSLI_

#include "params.hlsli"

//이것들은 Shader.cpp 에서 넣어준다.
struct VS_IN
{
    float3 localPos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;

};

//정점 셰이더
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    //좌표 조작
    // Translation은 하지 않고 Rotation만 적용한다
    float4 viewPos = mul(float4(input.localPos, 0), g_matView);
    float4 clipSpacePos = mul(viewPos, g_matProjection);//마지막이 아니고 동차좌표가 구해진다.

    //깊이 조작
    //스카이박스는 항상 깊이가 1이여야 하기 때문
    // w/w=1이기 때문에 항상 깊이가 1로 유지된다
    output.pos = clipSpacePos.xyww;
    output.uv = input.uv;

    return output;
}

//픽셀 셰이더
float4 PS_Main(VS_OUT input) : SV_Target
{
     float4 color = g_tex_0.Sample(g_sam_0, input.uv);
     return color;
}

#endif 