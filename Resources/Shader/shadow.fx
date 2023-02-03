#ifndef _SHADOW_FX_
#define _SHADOW_FX_

#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 clipPos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0.f;

    //이것고 clippos인데 따로 clippos를 또 받아놓는 이유는
    //해당 pos는 SV_Position시멘틱으로 지정되어 있어서 해당 pos가 PixelShader로 넘어갈때는 Pixel좌표로 변환되어 넘어가기 때문이다.
    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.clipPos = output.pos;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    //깊이값을 계산해서 넘겨준다.
    return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}

#endif